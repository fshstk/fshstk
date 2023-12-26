target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(${PROJECT_NAME} PRIVATE
  guiGlobals.cpp
  pathFromText.cpp
)

add_subdirectory(components)
