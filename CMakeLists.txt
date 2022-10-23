cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Cross-compile for x86 on Apple M1 machines:
set(CMAKE_OSX_ARCHITECTURES x86_64)

project(LightOrgan
    VERSION     0.0.1
    LANGUAGES   CXX)

include(Dependencies.cmake)

enable_testing()

juce_add_plugin(${CMAKE_PROJECT_NAME}
  COMPANY_NAME              "Fabian Hummel & Benedikt Mayrhofer"
  PLUGIN_MANUFACTURER_CODE  FhBm
  PLUGIN_CODE               LigO

  # NEEDS_MIDI_INPUT must be true if IS_SYNTH is true, or AU validation will fail:
  IS_SYNTH                  FALSE
  NEEDS_MIDI_INPUT          FALSE

  FORMATS                   AU VST3 Standalone
  COPY_PLUGIN_AFTER_BUILD   TRUE
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC
  JUCE_VST3_CAN_REPLACE_VST2=0

  # Disabling the splash screen is only legal if you purchase a JUCE license, or
  # if you license your project using the GNU Public License.
  # - https://www.gnu.org/licenses
  # - https://juce.com/juce-6-licence
  JUCE_DISPLAY_SPLASH_SCREEN=0
)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC
  juce::juce_audio_utils

  kfr
  kfr_dft
  kfr_io

  juce::juce_recommended_config_flags
  juce::juce_recommended_lto_flags
  juce::juce_recommended_warning_flags
)

add_subdirectory(src)
add_subdirectory(test)
