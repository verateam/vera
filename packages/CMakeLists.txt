
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

# source package
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${VERA_VERSION}")
set(CPACK_SOURCE_IGNORE_FILES
  "/\\\\.git"
  "/\\\\.mailmap$"
  "/\\\\.gitignore$"
  ".*~$"
  "\\\\.bak$"
  "\\\\.orig$"
  "/\\\\.DS_Store$"
  "/Thumbs\\\\.db"
  "/CMakeLists.txt.user$"
  ${CPACK_SOURCE_IGNORE_FILES})
# also ignore the build directories that may be in the source tree
file(GLOB_RECURSE caches "${CMAKE_SOURCE_DIR}/CMakeCache.txt")
foreach(c ${caches})
  get_filename_component(d "${c}" PATH)
  list(APPEND CPACK_SOURCE_IGNORE_FILES "^${d}/")
endforeach()

include(CPack)
