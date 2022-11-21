target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
  # Force including files as "section/<...>.h" to help readability:
  # ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
  Top.cpp
  Bottom.cpp
)
