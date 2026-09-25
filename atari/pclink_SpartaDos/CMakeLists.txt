cmake_minimum_required(VERSION 3.10)
project(AtariPclink LANGUAGES NONE)

# Find MADS (Mad-Assembler) 6502 assembler
find_program(MADS_EXECUTABLE
    NAMES mads MADS
    DOC "Path to MADS (Mad-Assembler) 6502 assembler executable"
)

set(MADS_FLAGS "" CACHE STRING "Additional command-line flags to pass to MADS assembler")

set(PCLINK_SHARED_DEPS
    "${CMAKE_CURRENT_SOURCE_DIR}/equ.asm"
    "${CMAKE_CURRENT_SOURCE_DIR}/printf.asm"
)

# Helper function to define an Atari 6502 MADS binary target
function(add_mads_binary TARGET_NAME SOURCE_ASM OUTPUT_NAME)
    set(OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_NAME}")

    if(MADS_EXECUTABLE)
        add_custom_command(
            OUTPUT "${OUTPUT_FILE}"
            COMMAND "${MADS_EXECUTABLE}" ${MADS_FLAGS} "-o:${OUTPUT_FILE}" "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_ASM}"
            DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_ASM}" ${PCLINK_SHARED_DEPS}
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            COMMENT "Assembling ${SOURCE_ASM} -> ${OUTPUT_NAME} using MADS"
            VERBATIM
        )

        add_custom_target(${TARGET_NAME}
            ALL
            DEPENDS "${OUTPUT_FILE}"
            SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_ASM}" ${PCLINK_SHARED_DEPS}
        )
    else()
        add_custom_target(${TARGET_NAME}
            COMMAND ${CMAKE_COMMAND} -E echo "Warning: MADS assembler not found. Cannot build ${OUTPUT_NAME} from ${SOURCE_ASM}. Specify -DMADS_EXECUTABLE=/path/to/mads"
            SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_ASM}" ${PCLINK_SHARED_DEPS}
        )
    endif()
endfunction()

# Define targets for PCLink CLI and PCLTEST suite
add_mads_binary(pclink_cli pclink.asm pclink.com)
add_mads_binary(pcltest_suite pcltest.asm pcltest.com)

# Aggregate target to build all Atari PCLink binaries
add_custom_target(pclink_atari_all
    DEPENDS pclink_cli pcltest_suite
)

if(MADS_EXECUTABLE)
    message(STATUS "MADS assembler found: ${MADS_EXECUTABLE}. Atari PCLink targets enabled.")
else()
    message(STATUS "MADS assembler not found. Atari PCLink binaries will be skipped (set -DMADS_EXECUTABLE to enable).")
endif()
