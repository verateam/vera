project(vera++)
cmake_minimum_required(VERSION 2.8)

set(VERA_VERSION "Vera++ 1.1.2 (Community Edition)")

add_subdirectory(src)
add_subdirectory(doc)
add_subdirectory(scripts)
add_subdirectory(profiles)

include(CTest)
add_subdirectory(tests)
