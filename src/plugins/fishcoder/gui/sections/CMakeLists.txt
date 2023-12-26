target_include_directories(${PROJECT_NAME} PRIVATE
  # Force including files as "section/<...>.h" to help readability:
  # ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(${PROJECT_NAME} PRIVATE
  Top.cpp
  Middle.cpp
  Bottom.cpp
)
