project(vera++)
cmake_minimum_required(VERSION 2.8)

set(VERA_MAJOR 1)
set(VERA_MINOR 2)
set(VERA_BUILD 0)
set(VERA_VERSION "${VERA_MAJOR}.${VERA_MINOR}.${VERA_BUILD}")

add_subdirectory(src)
add_subdirectory(doc)
add_subdirectory(scripts)
add_subdirectory(profiles)

include(CTest)
add_subdirectory(tests)

add_subdirectory(packages)
