target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
  GuiGlobals.cpp
  PathFromText.cpp
)

add_subdirectory(components)
add_subdirectory(sections)
