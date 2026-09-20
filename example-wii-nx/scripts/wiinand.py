"""Downloading and unpacking Wii system titles (channels, IOS, the Wii Menu).

Nintendo still serves every title a Wii could install from the update servers.
This module downloads one, unwraps its title key with the common key, decrypts
its contents and works out which content is the executable.

Nothing here stores Nintendo's data in the repository: callers write it to
folders that are not tracked.
"""

import os
import struct
import sys
import urllib.request

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from wiicrypto import COMMON_KEY, aes_cbc_decrypt  # noqa: E402

BASE_URL = "http://nus.cdn.shop.wii.com/ccs/download"
SIGNATURE = 0x140          # RSA-2048 signature + padding, before both TMD and ticket bodies


def fetch(title_id, name):
    url = f"{BASE_URL}/{title_id}/{name}"
    with urllib.request.urlopen(url, timeout=60) as response:
        if response.status != 200:
            raise SystemExit(f"{url}: HTTP {response.status}")
        return response.read()


def title_key(ticket):
    """Unwrap the title key from the ticket with the common key."""
    encrypted = ticket[0x1BF:0x1CF]
    title_id = ticket[0x1DC:0x1E4]
    return aes_cbc_decrypt(COMMON_KEY, title_id + b"\0" * 8, encrypted)


def contents(tmd):
    """(content id, index, size) for each content the title is made of."""
    count = struct.unpack(">H", tmd[SIGNATURE + 0x9E:SIGNATURE + 0xA0])[0]
    boot_index = struct.unpack(">H", tmd[SIGNATURE + 0xA0:SIGNATURE + 0xA2])[0]
    records = SIGNATURE + 0xA4
    listing = []
    for number in range(count):
        record = tmd[records + number * 36:records + number * 36 + 36]
        content_id, index, _type, size = struct.unpack(">IHHQ", record[:16])
        listing.append((content_id, index, size))
    return listing, boot_index


def dol_score(data):
    """How much this content looks like the title's real executable.

    A channel's "boot" content is often a small loader stub that pulls in the
    real program, so the metadata's boot index alone picks the wrong one. A real
    executable has several sections, sets the small-data registers in its startup
    code, and is the larger of the two.
    """
    if not looks_like_dol(data):
        return 0
    sizes = struct.unpack(">18I", data[0x90:0xD8])
    sections = sum(1 for size in sizes if size)
    score = 1 + sections
    if has_sda_setup(data):
        score += 10
    return score


def has_sda_setup(data):
    """True when the startup code loads r13/r2, as compiled game code does."""
    offsets = struct.unpack(">18I", data[0x00:0x48])
    addresses = struct.unpack(">18I", data[0x48:0x90])
    sizes = struct.unpack(">18I", data[0x90:0xD8])
    entry = struct.unpack(">I", data[0xE0:0xE4])[0]
    for index in range(7):          # text sections only
        if not sizes[index] or not addresses[index] <= entry < addresses[index] + sizes[index]:
            continue
        start = offsets[index] + (entry - addresses[index])
        window = data[start:start + 4096 * 4]
        for position in range(0, len(window) - 8, 4):
            instruction, following = struct.unpack(">II", window[position:position + 8])
            if instruction >> 26 != 15 or (instruction >> 16) & 31 != 0:
                continue
            target = (instruction >> 21) & 31
            if target in (2, 13) and following >> 26 in (14, 24) and (following >> 21) & 31 == target:
                return True
    return False


def looks_like_dol(data):
    if len(data) < 0x100:
        return False
    offsets = struct.unpack(">18I", data[0x00:0x48])
    addresses = struct.unpack(">18I", data[0x48:0x90])
    sizes = struct.unpack(">18I", data[0x90:0xD8])
    used = [i for i in range(18) if sizes[i]]
    if not used:
        return False
    for index in used:
        if offsets[index] + sizes[index] > len(data) or not 0x80000000 <= addresses[index] < 0x81800000:
            return False
    return True




def download(title_id, output, keep=False, quiet=False):
    """Download one title into `output`. Returns what was found."""
    title_id = title_id.lower().replace("-", "")
    os.makedirs(output, exist_ok=True)

    def say(text):
        if not quiet:
            print(text)

    tmd = fetch(title_id, "tmd")
    ticket = fetch(title_id, "cetk")
    key = title_key(ticket)
    listing, boot_index = contents(tmd)
    version = struct.unpack(">H", tmd[SIGNATURE + 0x9C:SIGNATURE + 0x9E])[0]
    say(f"   version {version}, {len(listing)} contents, boot index {boot_index}")

    if keep:
        with open(os.path.join(output, "tmd"), "wb") as handle:
            handle.write(tmd)
        with open(os.path.join(output, "cetk"), "wb") as handle:
            handle.write(ticket)

    executable, best_score, total = None, 0, 0
    for content_id, index, size in listing:
        raw = fetch(title_id, f"{content_id:08x}")
        iv = struct.pack(">H", index) + b"\0" * 14
        padded = len(raw) - (len(raw) % 16)
        data = aes_cbc_decrypt(key, iv, raw[:padded])[:size]
        total += size
        score = dol_score(data)
        kind = "executable" if score > 10 else ("DOL-like" if score else "data")
        boot = " (boot)" if index == boot_index else ""
        say(f"   content {content_id:08x} index {index}: {size} bytes, {kind}{boot}")
        if keep:
            with open(os.path.join(output, f"{content_id:08x}.app"), "wb") as handle:
                handle.write(data)
        if score > best_score:
            best_score, executable = score, data

    result = {"title_id": title_id, "version": version, "contents": len(listing),
              "boot_index": boot_index, "bytes": total, "executable": None}
    if executable is not None:
        path = os.path.join(output, "main.dol")
        with open(path, "wb") as handle:
            handle.write(executable)
        result["executable"] = path
        result["executable_bytes"] = len(executable)
    return result
