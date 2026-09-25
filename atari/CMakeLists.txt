# Atari 8-Bit subprojects
option(BUILD_ATARI_EXECUTABLES "Build Atari 8-bit executables (requires MADS assembler)" OFF)
if (BUILD_ATARI)
    set(BUILD_ATARI_EXECUTABLES ON)
endif()

if (BUILD_ATARI_EXECUTABLES)
    add_subdirectory(pclink_SpartaDos)
endif()
