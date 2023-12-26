project(FishSizer
  VERSION     ${CMAKE_PROJECT_VERSION}
  LANGUAGES   CXX)

juce_add_plugin(${PROJECT_NAME}
  PLUGIN_CODE               FSzr
  IS_SYNTH                  TRUE
  NEEDS_MIDI_INPUT          TRUE
  # (NEEDS_MIDI_INPUT must be true if IS_SYNTH is true, or AU validation will fail)

  COMPANY_NAME              ${FISH_COMPANY_NAME}
  PLUGIN_MANUFACTURER_CODE  ${FISH_PLUGIN_MANUFACTURER_CODE}
  FORMATS                   ${FISH_PLUGIN_FORMATS}
  COPY_PLUGIN_AFTER_BUILD   ${FISH_COPY_PLUGINS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  fishLib
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(${PROJECT_NAME} PRIVATE
  main.cpp
  PluginProcessor.cpp
  PluginState.cpp
)

add_subdirectory(wavetable)
