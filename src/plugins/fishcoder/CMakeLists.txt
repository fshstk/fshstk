project(FishCoder
  VERSION     ${CMAKE_PROJECT_VERSION}
  LANGUAGES   CXX)

juce_add_plugin(${PROJECT_NAME}
  COMPANY_NAME              fshstk
  PLUGIN_MANUFACTURER_CODE  Fstk
  PLUGIN_CODE               StEn

  # NEEDS_MIDI_INPUT must be true if IS_SYNTH is true, or AU validation will fail:
  IS_SYNTH                  FALSE
  NEEDS_MIDI_INPUT          FALSE

  FORMATS                   VST Standalone
  COPY_PLUGIN_AFTER_BUILD   TRUE
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  fishLib
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(${PROJECT_NAME} PRIVATE
  Main.cpp
  PluginEditor.cpp
  PluginProcessor.cpp
  PluginState.cpp
)

add_subdirectory(gui)
