add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wstrict-aliasing
    # -Werror
)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" )
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weffc++")
SET(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS}" )
SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS}" )
