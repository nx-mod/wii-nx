"""Wii crypto and constants shared by the scripts here.

A small, dependency-free AES-128 (verified against the FIPS-197 vector), since a
stock Python has no AES. It is slow - about a minute per MB - so openssl does the
work when it is installed, which is most of the time; the result is identical.
"""

import os
import shutil
import subprocess

# The Wii common key, which every disc's title key is wrapped with. Public
# knowledge and carried by Dolphin; no game data of any kind.
COMMON_KEY = bytes.fromhex("ebe42a225e8593e448d9c5457381aaf7")

CLUSTER = 0x8000          # one encrypted cluster on disc
CLUSTER_HASHES = 0x400    # its hash block
CLUSTER_DATA = 0x7C00     # its payload

SBOX = bytes.fromhex(
    "637c777bf26b6fc53001672bfed7ab76ca82c97dfa5947f0add4a2af9ca472c0"
    "b7fd9326363ff7cc34a5e5f171d8311504c723c31896059a071280e2eb27b275"
    "09832c1a1b6e5aa0523bd6b329e32f8453d100ed20fcb15b6acbbe394a4c58cf"
    "d0efaafb434d338545f9027f503c9fa851a3408f929d38f5bcb6da2110fff3d2"
    "cd0c13ec5f974417c4a77e3d645d197360814fdc222a908846eeb814de5e0bdb"
    "e0323a0a4906245cc2d3ac629195e479e7c8376d8dd54ea96c56f4ea657aae08"
    "ba78252e1ca6b4c6e8dd741f4bbd8b8a703eb5664803f60e613557b986c11d9e"
    "e1f8981169d98e949b1e87e9ce5528df8ca1890dbfe6426841992d0fb054bb16")
INV_SBOX = bytearray(256)
for _index, _value in enumerate(SBOX):
    INV_SBOX[_value] = _index
RCON = (0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36)


def _xtime(value):
    return ((value << 1) ^ 0x1B) & 0xFF if value & 0x80 else value << 1


def _mul(a, b):
    result = 0
    while b:
        if b & 1:
            result ^= a
        a = _xtime(a)
        b >>= 1
    return result


def _expand_key(key):
    words = [list(key[i * 4:i * 4 + 4]) for i in range(4)]
    for i in range(4, 44):
        temp = list(words[i - 1])
        if i % 4 == 0:
            temp = temp[1:] + temp[:1]
            temp = [SBOX[b] for b in temp]
            temp[0] ^= RCON[i // 4 - 1]
        words.append([words[i - 4][j] ^ temp[j] for j in range(4)])
    return words


def _decrypt_block(words, block):
    state = [list(block[row::4]) for row in range(4)]

    def add_round_key(rnd):
        for column in range(4):
            for row in range(4):
                state[row][column] ^= words[rnd * 4 + column][row]

    add_round_key(10)
    for rnd in range(9, -1, -1):
        for row in range(1, 4):
            state[row] = state[row][-row:] + state[row][:-row]
        for row in range(4):
            state[row] = [INV_SBOX[b] for b in state[row]]
        add_round_key(rnd)
        if rnd:
            for column in range(4):
                a = [state[row][column] for row in range(4)]
                state[0][column] = _mul(a[0], 14) ^ _mul(a[1], 11) ^ _mul(a[2], 13) ^ _mul(a[3], 9)
                state[1][column] = _mul(a[0], 9) ^ _mul(a[1], 14) ^ _mul(a[2], 11) ^ _mul(a[3], 13)
                state[2][column] = _mul(a[0], 13) ^ _mul(a[1], 9) ^ _mul(a[2], 14) ^ _mul(a[3], 11)
                state[3][column] = _mul(a[0], 11) ^ _mul(a[1], 13) ^ _mul(a[2], 9) ^ _mul(a[3], 14)
    return bytes(state[row][column] for column in range(4) for row in range(4))


def aes_cbc_decrypt_python(key, iv, data):
    words = _expand_key(key)
    out = bytearray(len(data))
    previous = iv
    for offset in range(0, len(data), 16):
        block = data[offset:offset + 16]
        plain = _decrypt_block(words, block)
        out[offset:offset + 16] = bytes(p ^ q for p, q in zip(plain, previous))
        previous = block
    return bytes(out)


_OPENSSL = shutil.which("openssl")


def _openssl_cbc_decrypt(key, iv, data):
    result = subprocess.run(
        [_OPENSSL, "enc", "-aes-128-cbc", "-d", "-nopad",
         "-K", key.hex(), "-iv", iv.hex()],
        input=data, stdout=subprocess.PIPE, stderr=subprocess.DEVNULL, check=False)
    return result.stdout if result.returncode == 0 and len(result.stdout) == len(data) else None


def aes_cbc_decrypt(key, iv, data):
    """AES-128-CBC, via openssl when available (the fallback is ~1 min/MB)."""
    if len(data) == 0:
        return b""
    if _OPENSSL is not None and len(data) >= 4096 and not os.environ.get("WIICRYPTO_PURE_PYTHON"):
        decrypted = _openssl_cbc_decrypt(key, iv, data)
        if decrypted is not None:
            return decrypted
    return aes_cbc_decrypt_python(key, iv, data)
