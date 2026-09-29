#!/usr/bin/env bash
# Drive the wii-nx pipeline from a Linux box: fetch what it builds against,
# translate a game, and cross-compile it into a Switch NRO.
#
#   wiinx.sh setup [NXVK.tar.xz]   sibling repos, deps, devkitPro, nxvk (stand-in without one)
#   wiinx.sh status                what is present, what is missing
#   wiinx.sh synthetic [DIR]       the no-disc test game: make, translate, build
#   wiinx.sh new-wad FILE.wad DIR  a project from a WAD you own (wads/<name>-nx)
#   wiinx.sh build PROJECT [--translate]   compile a project (translate first)
#   wiinx.sh globals PROJECT       re-derive globals.json, print why each value
#   wiinx.sh tests                 libdol-nx host tests (ctest)
#
# The workspace is the folder holding wii-nx; everything goes beside it:
#   libdol-nx libwii-nx libgc-nx wiicompiled-nx dawn-nx deps/
# Env: WS (workspace), DEVKITPRO (default /opt/devkitpro), JOBS (default nproc).
set -euo pipefail

here=$(cd "$(dirname "$0")" && pwd)
wiinx=$(cd "$here/../../.." && pwd)
WS=${WS:-$(dirname "$wiinx")}
DEVKITPRO=${DEVKITPRO:-/opt/devkitpro}
JOBS=${JOBS:-$(nproc)}
export DEVKITPRO
lib=$WS/libdol-nx
deps=$WS/deps

say() { printf '\n== %s\n' "$*"; }
die() { echo "wiinx.sh: $*" >&2; exit 1; }

# repo branch - the nx-mod branches this work is on.
SIBLINGS="libdol-nx:switch-testing
libwii-nx:switch-testing
libgc-nx:main
wiicompiled-nx:switch-testing
dawn-nx:switch"

# name url tag - what Aurora fetches as tarballs, which this network refuses
# (403), so each is a shallow git clone handed to FetchContent instead.
DEPS="abseil https://github.com/abseil/abseil-cpp 20240722.0
fmt https://github.com/fmtlib/fmt 11.1.4
freetype https://github.com/freetype/freetype VER-2-14-3
imgui https://github.com/ocornut/imgui v1.91.9b-docking
png https://github.com/pnggroup/libpng v1.6.58
tracy https://github.com/wolfpld/tracy a64b9a20294d59421a2f57aeca3c6383d8c48169
xxhash https://github.com/Cyan4973/xxHash v0.8.3
zlib https://github.com/madler/zlib v1.3.2
zstd https://github.com/facebook/zstd v1.5.7
sqlite-nx https://github.com/nx-mod/sqlite-nx 77103483f20795f744044f931596e7f4cc7858e0
sqlite-src https://github.com/sqlite/sqlite version-3.51.3"

clone_at() { # dir url ref
    local dir=$1 url=$2 ref=$3
    [ -e "$dir/.git" ] && return 0
    if [[ $ref =~ ^[0-9a-f]{40}$ ]]; then
        git init -q "$dir" && git -C "$dir" remote add origin "$url"
        git -C "$dir" fetch -q --depth 1 origin "$ref" && git -C "$dir" -c advice.detachedHead=false checkout -q FETCH_HEAD
    else
        git clone -q --depth 1 --branch "$ref" "$url" "$dir"
    fi
}

fetch_devkitpro() { # into $DEVKITPRO, from the devkitpro/devkita64 image's layers
    say "devkitPro into $DEVKITPRO (about 700 MB)"
    local tmp token manifest
    tmp=$(mktemp -d)
    token=$(curl -fsS "https://auth.docker.io/token?service=registry.docker.io&scope=repository:devkitpro/devkita64:pull" |
        python3 -c 'import sys,json;print(json.load(sys.stdin)["token"])')
    manifest=$(curl -fsS -H "Authorization: Bearer $token" \
        -H 'Accept: application/vnd.oci.image.index.v1+json' \
        https://registry-1.docker.io/v2/devkitpro/devkita64/manifests/latest |
        python3 -c 'import sys,json
for m in json.load(sys.stdin)["manifests"]:
    if m.get("platform",{}).get("architecture")=="amd64": print(m["digest"]); break')
    curl -fsS -H "Authorization: Bearer $token" -H 'Accept: application/vnd.oci.image.manifest.v1+json' \
        "https://registry-1.docker.io/v2/devkitpro/devkita64/manifests/$manifest" |
        python3 -c 'import sys,json;[print(l["digest"]) for l in json.load(sys.stdin)["layers"]]' > "$tmp/layers"
    mkdir -p "$tmp/root"
    while read -r layer; do
        curl -fsSL -H "Authorization: Bearer $token" \
            "https://registry-1.docker.io/v2/devkitpro/devkita64/blobs/$layer" |
            tar -xz -C "$tmp/root" --wildcards 'opt/devkitpro/*' 2>/dev/null || true
    done < "$tmp/layers"
    [ -f "$tmp/root/opt/devkitpro/cmake/Switch.cmake" ] || die "image had no opt/devkitpro"
    mkdir -p "$(dirname "$DEVKITPRO")"
    rm -rf "$DEVKITPRO" && mv "$tmp/root/opt/devkitpro" "$DEVKITPRO"
    rm -rf "$tmp"
}

cmd_setup() {
    local nxvk=${1:-}
    command -v dotnet >/dev/null || die "dotnet 8 is needed (apt-get install -y dotnet-sdk-8.0)"
    for t in cmake ninja git python3 tclsh curl; do
        command -v $t >/dev/null || die "$t is missing (apt-get install -y cmake ninja-build git python3 tcl curl)"
    done
    say "sibling repositories in $WS"
    while read -r entry; do
        local name=${entry%%:*} branch=${entry#*:}
        [ -e "$WS/$name/.git" ] || git clone -q --depth 1 --branch "$branch" "https://github.com/nx-mod/$name" "$WS/$name"
        echo "$name $(git -C "$WS/$name" rev-parse --abbrev-ref HEAD) $(git -C "$WS/$name" rev-parse --short HEAD)"
    done <<< "$SIBLINGS"

    say "dawn-nx submodules (only the ones a Switch build compiles)"
    # .gitmodules points at chromium.googlesource.com, which this network
    # refuses; the same commits are on GitHub.
    local dawn=$WS/dawn-nx sm repo sha
    while read -r sm repo; do
        sha=$(git -C "$dawn" ls-tree HEAD "$sm" | awk '{print $3}')
        [ -n "$(ls -A "$dawn/$sm" 2>/dev/null)" ] || { rm -rf "${dawn:?}/$sm"; clone_at "$dawn/$sm" "https://github.com/$repo" "$sha"; }
        echo "$sm ${sha:0:9}"
    done <<'EOS'
third_party/spirv-headers/src KhronosGroup/SPIRV-Headers
third_party/spirv-tools/src KhronosGroup/SPIRV-Tools
third_party/vulkan-headers/src KhronosGroup/Vulkan-Headers
third_party/vulkan-utility-libraries/src KhronosGroup/Vulkan-Utility-Libraries
EOS
    # Dawn pins abseil 2d78d7c, which GitHub's abseil does not serve. This
    # master commit builds, once taught that Horizon's pthread_t is a pointer
    # and that it keeps timezone names like newlib.
    local absl=$dawn/third_party/abseil-cpp
    [ -n "$(ls -A "$absl" 2>/dev/null)" ] || { rm -rf "$absl"
        clone_at "$absl" https://github.com/abseil/abseil-cpp 8c138bbb727731928af718dd9b80f562dc7363fd; }
    sed -i 's/return static_cast<pid_t>(pthread_self());/return static_cast<pid_t>(reinterpret_cast<uintptr_t>(pthread_self()));/' \
        "$absl/absl/base/internal/sysinfo.cc"
    grep -q '__EMSCRIPTEN__) || defined(__SWITCH__)' "$absl/absl/time/internal/cctz/src/time_zone_libc.cc" ||
        sed -i 's/    defined(__EMSCRIPTEN__)$/    defined(__EMSCRIPTEN__) || defined(__SWITCH__)/' \
            "$absl/absl/time/internal/cctz/src/time_zone_libc.cc"

    say "third-party sources in $deps"
    mkdir -p "$deps"
    while read -r name url ref; do clone_at "$deps/$name" "$url" "$ref"; echo "$name $ref"; done <<< "$DEPS"
    if [ ! -f "$deps/sqlite3/sqlite3.c" ]; then
        # sqlite.org is unreachable; build the amalgamation from the git mirror.
        mkdir -p "$deps/sqlite-bld" "$deps/sqlite3"
        (cd "$deps/sqlite-bld" && ../sqlite-src/configure >/dev/null && make -s sqlite3.c >/dev/null)
        cp "$deps/sqlite-bld/sqlite3.c" "$deps/sqlite-bld/sqlite3.h" "$deps/sqlite-bld/sqlite3ext.h" "$deps/sqlite3/"
    fi

    [ -f "$DEVKITPRO/cmake/Switch.cmake" ] || fetch_devkitpro
    local portlib=$DEVKITPRO/portlibs/switch/lib
    if [ -n "$nxvk" ]; then
        # nxvk-switch-portlib-<ver>.tar.xz, from github.com/nx-mod/vulkan-nx releases.
        say "nxvk from $nxvk"
        mkdir -p "$DEVKITPRO/portlibs/switch"
        tar -xJf "$nxvk" -C "$DEVKITPRO/portlibs/switch"
    elif [ ! -f "$portlib/libnvk.a" ]; then
        # Real nxvk comes from github.com/nx-mod/nxvk releases, which this
        # network cannot reach. Empty archives let everything compile; the
        # final link then stops on vk_icdGetInstanceProcAddr.
        say "nxvk not installed: empty stand-in libnvk.a (the NRO will not link)"
        "$DEVKITPRO/devkitA64/bin/aarch64-none-elf-ar" rc "$portlib/libnvk.a"
        "$DEVKITPRO/devkitA64/bin/aarch64-none-elf-ar" rc "$portlib/libnvk_support.a"
    fi
    cmd_status
}

cmd_status() {
    say "status ($WS)"
    while read -r entry; do
        local name=${entry%%:*}
        [ -e "$WS/$name/.git" ] && echo "ok   $name ($(git -C "$WS/$name" rev-parse --abbrev-ref HEAD))" || echo "MISS $name"
    done <<< "$SIBLINGS"
    [ -f "$DEVKITPRO/cmake/Switch.cmake" ] && echo "ok   devkitPro $DEVKITPRO" || echo "MISS devkitPro ($DEVKITPRO)"
    if [ -s "$DEVKITPRO/portlibs/switch/lib/libnvk.a" ] && [ "$(stat -c %s "$DEVKITPRO/portlibs/switch/lib/libnvk.a")" -gt 8 ]; then
        echo "ok   nxvk"
    else
        echo "STUB nxvk (compiles, does not link)"
    fi
    command -v dotnet >/dev/null && echo "ok   dotnet $(dotnet --version)" || echo "MISS dotnet"
    [ -f "$deps/sqlite3/sqlite3.c" ] && echo "ok   deps" || echo "MISS deps"
}

# cmake/game for PROJECT into PROJECT/build, deps from $deps.
configure_build() { # project target
    local game=$1 target=$2
    [ -f "$DEVKITPRO/cmake/Switch.cmake" ] || die "no devkitPro at $DEVKITPRO (wiinx.sh setup)"
    say "configuring $(basename "$game")"
    cmake -S "$lib/cmake/game" -B "$game/build" -G Ninja \
        -DCMAKE_TOOLCHAIN_FILE="$DEVKITPRO/cmake/Switch.cmake" \
        -DWIINX_GAME_DIR="$game" -DWIINX_DAWN_DIR="$WS/dawn-nx" \
        -DWIINX_AURORA_DIR="$WS/wiicompiled-nx/aurora-main" \
        -DAURORA_SQLITE_NX_DIR="$deps/sqlite-nx" \
        -DFETCHCONTENT_SOURCE_DIR_ABSEIL-CPP="$deps/abseil" \
        -DFETCHCONTENT_SOURCE_DIR_FMT="$deps/fmt" \
        -DFETCHCONTENT_SOURCE_DIR_FREETYPE="$deps/freetype" \
        -DFETCHCONTENT_SOURCE_DIR_IMGUI="$deps/imgui" \
        -DFETCHCONTENT_SOURCE_DIR_PNG="$deps/png" \
        -DFETCHCONTENT_SOURCE_DIR_SQLITE3="$deps/sqlite3" \
        -DFETCHCONTENT_SOURCE_DIR_TRACY="$deps/tracy" \
        -DFETCHCONTENT_SOURCE_DIR_XXHASH="$deps/xxhash" \
        -DFETCHCONTENT_SOURCE_DIR_ZLIB="$deps/zlib" \
        -DFETCHCONTENT_SOURCE_DIR_ZSTD="$deps/zstd" > "$game/build.configure.log" 2>&1 ||
        { tail -30 "$game/build.configure.log"; die "configure failed ($game/build.configure.log)"; }
    say "building $target (-j$JOBS; log $game/build.log)"
    set +e
    nice -n 10 cmake --build "$game/build" -j "$JOBS" --target "$target" > "$game/build.log" 2>&1
    local rc=$?
    set -e
    local nro
    nro=$(find "$game/build" -name '*.nro' -newer "$game/build.configure.log" 2>/dev/null | head -1)
    if [ $rc -eq 0 ] && [ -n "$nro" ]; then
        echo "NRO: $nro ($(stat -c %s "$nro") bytes)"
        return 0
    fi
    local undef
    undef=$(grep -o "undefined reference to \`[^']*'" "$game/build.log" | sort -u)
    if [ -n "$undef" ] && ! grep -q 'error:' <(grep -v 'ld returned' "$game/build.log"); then
        echo "COMPILED; link stopped on:"; echo "$undef" | sed 's/^/  /'
        echo "(expected with the nxvk stand-in: every object built, only the Vulkan driver is missing)"
        return 3
    fi
    grep -E 'error:|FAILED' "$game/build.log" | head -20
    die "build failed ($game/build.log)"
}

cmd_synthetic() {
    local dir=${1:-/tmp/wiinx-synthetic}
    dir=$(mkdir -p "$dir" && cd "$dir" && pwd)
    say "synthetic game in $dir"
    "$lib/tests/synthetic/make-game" "$dir"
    "$lib/tools/wiinx-translate" "$dir"
    configure_build "$dir" synthetic_nro
}

cmd_new_wad() {
    [ $# -eq 2 ] || die "new-wad FILE.wad DIR"
    "$lib/tools/wiinx-new-title" "$1" "$2"
}

cmd_build() {
    [ $# -ge 1 ] || die "build PROJECT [--translate]"
    local game
    game=$(cd "$1" && pwd)
    [ -f "$game/recomp.yml" ] || die "no recomp.yml in $game"
    if [ "${2:-}" = --translate ]; then
        say "translating $(basename "$game")"
        "$lib/tools/wiinx-translate" "$game" --threads "$JOBS"
    fi
    configure_build "$game" "$(basename "$game")_nro"
}

cmd_globals() {
    [ $# -eq 1 ] || die "globals PROJECT"
    "$lib/tools/wiinx-find-globals" "$(cd "$1" && pwd)" --why
}

cmd_tests() {
    say "libdol-nx host tests"
    cmake -S "$lib" -B "$lib/build" -G Ninja > /dev/null
    cmake --build "$lib/build" -j "$JOBS" > /dev/null
    (cd "$lib/build" && ctest --output-on-failure 2>&1 | tail -5)
}

case ${1:-} in
    setup) shift; cmd_setup "$@" ;;
    status) cmd_status ;;
    synthetic) shift; cmd_synthetic "$@" ;;
    new-wad) shift; cmd_new_wad "$@" ;;
    build) shift; cmd_build "$@" ;;
    globals) shift; cmd_globals "$@" ;;
    tests) cmd_tests ;;
    *) sed -n '2,17p' "$0" | sed 's/^# \{0,1\}//'; exit 2 ;;
esac
