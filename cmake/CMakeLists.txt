configure_file(${CMAKE_CURRENT_SOURCE_DIR}/vera++-config-version.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/vera++-config-version.cmake)

if(WIN)
  set(destdir cmake/)
else()
  set(destdir lib/vera++/)
endif()
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/vera++-config-version.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/vera++-config.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/use_vera++.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/test_wrapper.cmake.in
  DESTINATION ${destdir})
