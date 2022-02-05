 file(GLOB_RECURSE SOURCES "lex_data_include/*.c" "*.c")

idf_component_register(SRCS "${SOURCES}"
    INCLUDE_DIRS "." "src"
    REQUIRES usenglish)

