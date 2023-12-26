project(fishLib
    VERSION     ${CMAKE_PROJECT_VERSION}
    LANGUAGES   CXX)

add_library(${PROJECT_NAME} STATIC)

target_compile_options(${PROJECT_NAME} PUBLIC
  -Wall
  -Wextra
  -Wpedantic
  -Werror
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  juce::juce_audio_utils
  Fonts

  fmt
  
  juce::juce_recommended_config_flags
  juce::juce_recommended_lto_flags
  juce::juce_recommended_warning_flags
)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(${PROJECT_NAME} PRIVATE
)

add_subdirectory(utils)
