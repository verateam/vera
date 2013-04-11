find_program(PANDOC pandoc)

if(NOT PANDOC)
  message(WARNING "The documentation won't be built because pandoc was not found.")
  return()
endif()

set(wiki "${CMAKE_SOURCE_DIR}/wiki")
if(NOT EXISTS "${wiki}/Introduction.md")
  message(WARNING "The documentation won't be built because the wiki directory is empty.")
  message(WARNING "Run 'git submodule update --init' to fix that.")
  return()
endif()

set(inputs
  ${wiki}/Introduction.md
  # no need for the build page in the final doc
  # ${wiki}/Building.md
  ${wiki}/Running.md
  ${wiki}/Rules.md
  ${wiki}/Transformations.md
  ${wiki}/ScriptAPI.md
)
add_custom_command(OUTPUT vera++.1
  COMMAND ${PANDOC} -s
    ${CMAKE_CURRENT_SOURCE_DIR}/manpage.md
    ${inputs}
    -o vera++.1
  DEPENDS ${PANDOC} ${CMAKE_CURRENT_SOURCE_DIR}/manpage.md ${inputs}
)
add_custom_command(OUTPUT vera++.html
  COMMAND ${PANDOC} -s
    # --toc --toc-depth=1
    -H ${CMAKE_CURRENT_SOURCE_DIR}/style.css
    ${inputs}
    -o vera++.html
  DEPENDS ${PANDOC} ${CMAKE_CURRENT_SOURCE_DIR}/style.css ${inputs}
)
add_custom_target(doc ALL DEPENDS vera++.1 vera++.html)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/vera++.1 DESTINATION share/man/man1)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/vera++.html DESTINATION share/vera++/doc)
