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

# don't put the full path in this list because it breaks the doc generation
# on windows. Instead we run the command in the wiki directory
set(inputs
  Introduction.md
  # no need for the build page in the final doc
  # ${wiki}/Building.md
  Running.md
  Rules.md
  Transformations.md
  ScriptAPI.md
  Changes.md
)
# generate the list of inputs with the full path to deal properly with
# dependencies in add_custom_command
set(inputs_full_path)
foreach(i ${inputs})
  list(APPEND inputs_full_path "${wiki}/${i}")
endforeach()

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/vera++.1
  COMMAND ${PANDOC} -s
    ${CMAKE_CURRENT_SOURCE_DIR}/manpage.md
    ${inputs}
    -o ${CMAKE_CURRENT_BINARY_DIR}/vera++.1
  DEPENDS ${PANDOC} ${CMAKE_CURRENT_SOURCE_DIR}/manpage.md ${inputs_full_path}
  WORKING_DIRECTORY "${wiki}"
)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/vera++.html
  COMMAND ${PANDOC} -s
    # --toc --toc-depth=1
    -H ${CMAKE_CURRENT_SOURCE_DIR}/style.css
    ${inputs}
    -o ${CMAKE_CURRENT_BINARY_DIR}/vera++.html
  DEPENDS ${PANDOC} ${CMAKE_CURRENT_SOURCE_DIR}/style.css ${inputs_full_path}
  WORKING_DIRECTORY "${wiki}"
)
add_custom_target(doc ALL DEPENDS
  ${CMAKE_CURRENT_BINARY_DIR}/vera++.1
  ${CMAKE_CURRENT_BINARY_DIR}/vera++.html)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/vera++.1 DESTINATION share/man/man1)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/vera++.html DESTINATION share/vera++/doc)
