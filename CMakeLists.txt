project(vera++)
cmake_minimum_required(VERSION 2.8.6)

set(VERA_MAJOR 1)
set(VERA_MINOR 3)
set(VERA_BUILD 0)
set(VERA_VERSION "${VERA_MAJOR}.${VERA_MINOR}.${VERA_BUILD}")

add_subdirectory(src)
add_subdirectory(doc)
add_subdirectory(rules)
add_subdirectory(transformations)
add_subdirectory(profiles)
add_subdirectory(packages)
add_subdirectory(cmake)

include(CTest)
add_subdirectory(tests)

# pass a few specific options to CTest. CTestCustom.cmake must be at the root of
# the binary dir
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/CTestCustom.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/CTestCustom.cmake
  COPYONLY)
