project(JX11
  VERSION     ${CMAKE_PROJECT_VERSION}
  LANGUAGES   CXX)

juce_add_plugin(${PROJECT_NAME}
  PLUGIN_CODE               Jx11
  IS_SYNTH                  TRUE
  NEEDS_MIDI_INPUT          TRUE
  # (NEEDS_MIDI_INPUT must be true if IS_SYNTH is true, or AU validation will fail)

  PLUGIN_MANUFACTURER_CODE  ${FSH_PLUGIN_MANUFACTURER_CODE}
  FORMATS                   ${FSH_PLUGIN_FORMATS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  fshlib
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(${PROJECT_NAME} PRIVATE
  main.cpp
  PluginProcessor.cpp
  PluginState.cpp
  MidiEvent.cpp
  Synth.cpp
  Voice.cpp
  WhiteNoise.cpp
)
