
# binary package
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "The vera++ team")
set(CPACK_PACKAGE_CONTACT "none")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Programmable verification and analysis tool for C++")
set(CPACK_PACKAGE_VERSION_MAJOR "${VERA_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${VERA_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${VERA_BUILD}")
set(CPACK_PACKAGE_VERSION "${VERA_VERSION}")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.txt")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE_1_0.txt")
# set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/vera++.bmp")
# set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/vera++.ico")
# set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/vera++.ico")
if(TARGET doc)
  set(CPACK_NSIS_MENU_LINKS "share\\\\vera++\\\\doc\\\\vera++.html" "documentation")
endif()
set(CPACK_NSIS_MODIFY_PATH "ON")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")
include(CPack)


# source package
find_package(PythonInterp)

if(GITCOMMAND
  AND PYTHON_EXECUTABLE
  AND EXISTS ${CMAKE_SOURCE_DIR}/.git)
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
endif()
