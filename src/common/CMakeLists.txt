####################################################################################################
#                ██████          █████                              █████    █████                 #
#               ███░░███        ░░███                              ░░███    ░░███                  #
#              ░███ ░░░   █████  ░███████      ██    ██     █████  ███████   ░███ █████            #
#             ███████    ███░░   ░███░░███    ░░    ░░     ███░░  ░░░███░    ░███░░███             #
#            ░░░███░    ░░█████  ░███ ░███                ░░█████   ░███     ░██████░              #
#              ░███      ░░░░███ ░███ ░███                 ░░░░███  ░███ ███ ░███░░███             #
#              █████     ██████  ████ █████    ██    ██    ██████   ░░█████  ████ █████            #
#            ░░░░░     ░░░░░░  ░░░░ ░░░░░    ░░    ░░    ░░░░░░     ░░░░░  ░░░░ ░░░░░              #
#                                                                                                  #
#           fantastic  spatial  holophonic                software    tool    kit                  #
#                                                                                                  #
#                                   copyright (c) fabian hummel                                    #
#                                      www.github.com/fshstk                                       #
#                                          www.fshstk.com                                          #
#                                                                                                  #
#        this file is part of the fantastic spatial holophonic software toolkit (fsh::stk)         #
# fsh::stk is free software: it is provided under the terms of the gnu general public license v3.0 #
#                                   www.gnu.org/licenses/gpl-3.0                                   #
####################################################################################################

project(fshlib
    VERSION     ${CMAKE_PROJECT_VERSION}
    LANGUAGES   CXX)

add_library(fshlib STATIC)

target_compile_options(${PROJECT_NAME} PUBLIC
  -Wall
)

if(NOT MSVC)
target_compile_options(${PROJECT_NAME} PUBLIC
  -Wextra
  -Wpedantic
  -Werror
)
endif()

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
  spdlog::spdlog
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
