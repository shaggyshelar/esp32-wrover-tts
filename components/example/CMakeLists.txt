 if(CONFIG_FOO_ENABLE_BAR)
     list(APPEND srcs "bar.c")
 endif()

 file(GLOB_RECURSE SOURCES src/*.c)

idf_component_register(SRCS "${SOURCES}"
    INCLUDE_DIRS . src
    REQUIRES main)

