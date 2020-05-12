#########################################
## generate compile database
#########################################
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# uncomment below to have a verbose build process
SET(CMAKE_VERBOSE_MAKEFILE ON)

#########################################
## Global compiler settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wstrict-aliasing
    -Werror
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" )
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weffc++")
SET(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS}" )
SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS}" )

