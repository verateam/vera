
find_package(TCL REQUIRED)
include_directories(SYSTEM ${TCL_INCLUDE_DIRS})
link_directories(${TCL_LIBRARY_DIRS})

find_package(Boost COMPONENTS system wave REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

configure_file(config.h.in config.h @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB_RECURSE srcs *.cpp)
add_executable(vera++ ${srcs})
target_link_libraries(vera++
  ${TCL_LIBRARY}
  ${Boost_LIBRARIES})

install(TARGETS vera++ DESTINATION bin)
