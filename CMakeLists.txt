# wii-nx: one build for a Wii game on Switch.
#
#   cmake -S . -B build -G Ninja \
#         -DCMAKE_TOOLCHAIN_FILE=$DEVKITPRO/cmake/Switch.cmake \
#         -DWIINX_GAME=mkwii-nx
#   cmake --build build
#
# It puts the pieces together in order: the libraries, then the engine, then the
# game, ending in <game>.nro laid out for sdmc:/wii-nx/games/<game>/.
#
# Each library builds on its own too (that is the point of them), so this only
# composes; it holds no logic of its own. Libraries not checked out as submodules
# can be pointed at with -DWIINX_<NAME>_DIR=<path>.
cmake_minimum_required(VERSION 3.22)

# Release is the only configuration the engine supports, and the one the flags
# below assume. Set before project() so it reaches the compiler flags.
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif ()

project(wii-nx LANGUAGES C CXX ASM)

if (NOT CMAKE_SYSTEM_NAME STREQUAL "NintendoSwitch")
    message(FATAL_ERROR
        "wii-nx builds for Nintendo Switch. Configure with:\n"
        "  -DCMAKE_TOOLCHAIN_FILE=\$DEVKITPRO/cmake/Switch.cmake")
endif ()

set(WIINX_GAME "mkwii-nx" CACHE STRING
    "Which game to build: a folder in games/, or empty for the libraries alone")

# Each piece, wherever it lives.
foreach (component wiicompiled-nx libdol-nx libwii-nx aurora-nx dawn-nx nxvk sqlite-nx)
    string(TOUPPER "${component}" upper)
    string(REPLACE "-" "_" upper "${upper}")
    set(WIINX_${upper}_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${component}" CACHE PATH
        "${component} checkout")
endforeach ()

if (NOT EXISTS "${WIINX_DAWN_NX_DIR}/CMakeLists.txt")
    message(FATAL_ERROR
        "dawn-nx is missing from ${WIINX_DAWN_NX_DIR}.\n"
        "Clone with --recursive, or run: git submodule update --init dawn-nx")
endif ()

# ---------------------------------------------------------------------------
# Libraries
# ---------------------------------------------------------------------------

# The Wii's NAND formats: settings, Miis, saves. Used by the runtime and by the
# launcher, and buildable on its own.
if (EXISTS "${WIINX_LIBDOL_NX_DIR}/CMakeLists.txt")
    add_subdirectory("${WIINX_LIBDOL_NX_DIR}" libdol-nx)
endif ()

# WebGPU. Dawn builds the game itself when it is the top-level project; here it
# is not, so it builds only itself and we add the game below.
set(DAWN_BUILD_MONOLITHIC_LIBRARY STATIC CACHE STRING "" FORCE)
set(DAWN_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(DAWN_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(DAWN_BUILD_PROTOBUF OFF CACHE BOOL "" FORCE)
set(DAWN_FETCH_DEPENDENCIES OFF CACHE BOOL "" FORCE)
set(DAWN_ENABLE_VULKAN ON CACHE BOOL "" FORCE)
set(DAWN_USE_GLFW OFF CACHE BOOL "" FORCE)
set(DAWN_USE_X11 OFF CACHE BOOL "" FORCE)
set(DAWN_USE_WAYLAND OFF CACHE BOOL "" FORCE)
set(TINT_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(TINT_BUILD_CMD_TOOLS OFF CACHE BOOL "" FORCE)
add_subdirectory("${WIINX_DAWN_NX_DIR}" dawn-build)

# ---------------------------------------------------------------------------
# Engine and game
# ---------------------------------------------------------------------------

if (WIINX_GAME)
    set(WIINX_GAME_DIR "${CMAKE_CURRENT_SOURCE_DIR}/games/${WIINX_GAME}")
    if (NOT EXISTS "${WIINX_GAME_DIR}/recomp.yml")
        message(FATAL_ERROR
            "No game at ${WIINX_GAME_DIR}.\n"
            "Available: see games/, or -DWIINX_GAME= to build the libraries alone.")
    endif ()
    if (NOT EXISTS "${WIINX_GAME_DIR}/generated/build_shards/shards.cmake")
        message(FATAL_ERROR
            "${WIINX_GAME} has no translated code yet.\n"
            "Extract your disc and translate it first:\n"
            "  games/${WIINX_GAME}/scripts/extract <your disc image>\n"
            "  ../libdol-nx/tools/wiinx-translate games/${WIINX_GAME}")
    endif ()

    # Aurora (GX on WebGPU) reuses the Dawn added above rather than fetching its
    # own; its CMake checks for the target before doing so.
    set(AURORA_ENABLE_GX ON CACHE BOOL "" FORCE)
    set(AURORA_ENABLE_DVD OFF CACHE BOOL "" FORCE)
    set(AURORA_ENABLE_CARD OFF CACHE BOOL "" FORCE)
    set(AURORA_DAWN_PROVIDER "vendor" CACHE STRING "" FORCE)
    set(AURORA_SDL3_PROVIDER "vendor" CACHE STRING "" FORCE)
    set(AURORA_SQLITE_NX_DIR "${WIINX_SQLITE_NX_DIR}" CACHE PATH "" FORCE)
    add_subdirectory("${WIINX_WIICOMPILED_NX_DIR}/aurora-main" aurora-build)

    # The engine, pointed at this game's translated code.
    set(MKW_BUILD_PRODUCTS ON CACHE BOOL "" FORCE)
    set(MKW_GAME_WORKSPACE_DIR "${WIINX_GAME_DIR}" CACHE PATH "" FORCE)
    add_subdirectory("${WIINX_WIICOMPILED_NX_DIR}/runtime" runtime-build)

    if (TARGET WiiCompiled)
        # The graphics path ends at NVK; dawn-nx knows how it has to be linked.
        dawn_switch_link_nvk(WiiCompiled)
        # Every thread gets a 4 MiB stack instead of libnx's 128 KiB default:
        # the shader compiler overflows the default.
        target_link_libraries(WiiCompiled PRIVATE -Wl,--wrap=pthread_create)

        # The folder the game is copied to on the SD card names it, so the
        # runtime finds its own config, disc, caches and logs.
        target_compile_definitions(WiiCompiled PRIVATE WIINX_GAME_DIR="${WIINX_GAME}")

        nx_generate_nacp(${WIINX_GAME}.nacp
            NAME "${WIINX_GAME}" AUTHOR "nx-mod" VERSION "1.0.0")
        nx_create_nro(${WIINX_GAME}_nro TARGET WiiCompiled
            OUTPUT ${WIINX_GAME}.nro NACP ${WIINX_GAME}.nacp)
        message(STATUS "wii-nx: ${WIINX_GAME} -> ${WIINX_GAME}.nro")
    endif ()
else ()
    message(STATUS "wii-nx: libraries only (-DWIINX_GAME=<folder in games> to build a game)")
endif ()
