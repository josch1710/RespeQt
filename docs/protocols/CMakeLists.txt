find_program(TYPST_EXECUTABLE typst DOC "Typst compiler")

if(TYPST_EXECUTABLE)
    set(TYPST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/respeqt_protocols.typst")
    set(TYPST_PDF "${CMAKE_CURRENT_BINARY_DIR}/respeqt_protocols.pdf")

    add_custom_command(
        OUTPUT "${TYPST_PDF}"
        COMMAND "${TYPST_EXECUTABLE}" compile "${TYPST_SRC}" "${TYPST_PDF}"
        DEPENDS "${TYPST_SRC}"
        COMMENT "Compiling ${TYPST_SRC} to PDF with Typst"
        VERBATIM
    )

    add_custom_target(respeqt_protocols_doc ALL
        DEPENDS "${TYPST_PDF}"
        SOURCES "${TYPST_SRC}"
    )
    message(STATUS "Typst found. Documentation will be built.")
else()
    message(STATUS "Typst not found. Documentation will not be built.")
endif()
