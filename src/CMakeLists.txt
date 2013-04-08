
find_package(TCL REQUIRED)
include_directories(SYSTEM ${TCL_INCLUDE_DIRS})
link_directories(${TCL_LIBRARY_DIRS})

find_package(Boost COMPONENTS program_options system wave REQUIRED)
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

########## check the coding style with vera++ ##############

# We parallelize the style checking by running one instance of vera++ per
# source directory.
# So first lets find the source directories
set(dirs)
file(GLOB_RECURSE srcs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h *.cpp)
foreach(s ${srcs})
  get_filename_component(d ${s} PATH)
  if("${d}" STREQUAL "")
    # use '.' to avoid loosing the empty string
    list(APPEND dirs ".")
  else()
    list(APPEND dirs ${d})
  endif()
endforeach()
list(REMOVE_DUPLICATES dirs)
# Two custom targets will be created:
# * style_reports is run as part of the build, and is not rerun unless one of
# the file checked is modified;
# * style must be explicitely called (make style) and is rerun even if the files
# to check have not been modified. To achieve this behavior, the commands used
# in this target pretend to produce a file without actually producing it.
# Because the output file is not there after the run, the command will be rerun
# again at the next target build.
set(xmlreports)
set(noreports)
foreach(d ${dirs})
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${d})
  file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/${d}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/${d}/*.cpp")
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${d}/vera.xml
    COMMAND vera++
      --root ${CMAKE_SOURCE_DIR}
      --std-report=-
      --warning
      --xml-report=${CMAKE_CURRENT_BINARY_DIR}/${d}/vera.xml
      ${srcs}
    DEPENDS ${srcs}
    COMMENT "Checking style in ${d}"
  )
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${d}/noreport
    COMMAND vera++
      --root ${CMAKE_SOURCE_DIR}
      --std-report=-
      --warning
      ${srcs}
    DEPENDS ${srcs}
    COMMENT "Checking style in ${d}"
  )
  list(APPEND xmlreports ${CMAKE_CURRENT_BINARY_DIR}/${d}/vera.xml)
  list(APPEND noreports ${CMAKE_CURRENT_BINARY_DIR}/${d}/noreport)
endforeach()
# Create the custom targets that will trigger the custom command created
# previously
add_custom_target(style_reports ALL DEPENDS ${xmlreports})
add_custom_target(style DEPENDS ${noreports})
# And add a dependency on vera++, because we are building vera++ and running it
# in the same build process
add_dependencies(style_reports vera++)
add_dependencies(style vera++)
