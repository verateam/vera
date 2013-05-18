include(WriteBasicConfigVersionFile)
write_basic_config_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/vera++-config-version.cmake
  VERSION ${VERA_VERSION}
  COMPATIBILITY AnyNewerVersion)

if(WIN)
  set(destdir cmake/)
else()
  set(destdir lib/vera++/)
endif()
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/vera++-config-version.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/vera++-config.cmake
  DESTINATION ${destdir})
