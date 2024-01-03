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
  Fonts
  fmt
  juce::juce_audio_utils
  juce::juce_dsp
  juce::juce_recommended_config_flags
  juce::juce_recommended_lto_flags
  juce::juce_recommended_warning_flags
)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(${PROJECT_NAME} PRIVATE
)

add_subdirectory(fx)
add_subdirectory(gui)
add_subdirectory(utils)
add_subdirectory(wavetable)
