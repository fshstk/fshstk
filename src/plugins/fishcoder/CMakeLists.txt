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

target_compile_definitions(${PROJECT_NAME} PUBLIC
  # Uncomment this if you want to build VST3 plugins without the VST2 SDK:
  # JUCE_VST3_CAN_REPLACE_VST2=0

  # Disabling the splash screen is only legal if you purchase a JUCE license, or
  # if you license your project using the GNU Public License.
  # - https://www.gnu.org/licenses
  # - https://juce.com/juce-6-licence
  JUCE_DISPLAY_SPLASH_SCREEN=0
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  fishLib

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
  Main.cpp
  PluginEditor.cpp
  PluginProcessor.cpp
  PluginState.cpp
)

add_subdirectory(gui)
