
include(UseLATEX)

if(NOT "${LATEX_COMPILER}" EQUAL "LATEX_COMPILER-NOTFOUND")
#project(Protocols NONE)
add_latex_document(
        respeqt_protocols.tex
        #BIBFILES references.bib
        FORCE_PDF
)
endif()