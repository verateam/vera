find_package(PythonInterp)

if(NOT GITCOMMAND)
  return()
endif()
if(NOT PYTHON_EXECUTABLE)
  return()
endif()
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/.git)
  return()
endif()

# get the git-archive-all script that allows to package the submodules within
# the main sources
file(DOWNLOAD
  https://raw.github.com/Kentzo/git-archive-all/75bd629ef3dde58c83e4af4c92978e753e6c6b72/git-archive-all
  ${CMAKE_CURRENT_BINARY_DIR}/git-archive-all
  EXPECTED_MD5 fe37e10ef385718b5150b154c6f224ad
)
# and create the sources archive
set(dest ${CMAKE_CURRENT_BINARY_DIR}/vera++-${VERA_VERSION}.tar.gz)
add_custom_command(OUTPUT ${dest}
  COMMAND ${PYTHON_EXECUTABLE}
  ${CMAKE_CURRENT_BINARY_DIR}/git-archive-all ${dest}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
add_custom_target(src DEPENDS ${dest})
