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
#           fantastic  spatial  holophonic               synthesis    tool    kit                  #
#                                                                                                  #
#                                   copyright (c) fabian hummel                                    #
#                                      www.github.com/fshstk                                       #
#                                          www.fshstk.com                                          #
#                                                                                                  #
#        this file is part of the fantastic spatial holophonic synthesis toolkit (fsh::stk)        #
# fsh::stk is free software: it is provided under the terms of the gnu general public license v3.0 #
#                                   www.gnu.org/licenses/gpl-3.0                                   #
####################################################################################################

project(fshlib
    VERSION     ${FSH_PROJECT_VERSION}
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

include(${CMAKE_SOURCE_DIR}/cmake/GitCommitHash.cmake)
get_git_commit_hash(GIT_COMMIT_HASH)

target_compile_definitions(${PROJECT_NAME} PUBLIC
  # Uncomment this if you want to build VST3 plugins without the VST2 SDK:
  JUCE_VST3_CAN_REPLACE_VST2=0

  # This one's important because it gets rid of the "gtk header not found" error
  # when compiling under Ubuntu:
  JUCE_WEB_BROWSER=0

  # Disabling the splash screen is only legal if you purchase a JUCE license, or
  # if you license your project using the GNU Public License.
  # - https://www.gnu.org/licenses
  # - https://juce.com/juce-7-licence
  JUCE_DISPLAY_SPLASH_SCREEN=0

  # Can be used to display commit hash in the GUI (defaults to empty string if
  # the current commit is a tagged release):
  FSH_COMMIT_HASH="${GIT_COMMIT_HASH}"
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  fmt
  spdlog::spdlog
  juce::juce_audio_utils
  juce::juce_dsp
  juce::juce_recommended_config_flags
  juce::juce_recommended_lto_flags
  juce::juce_recommended_warning_flags
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  FontData
  BackgroundData
)

add_subdirectory(fx)
add_subdirectory(gui)
add_subdirectory(plugin)
add_subdirectory(util)
add_subdirectory(synth)
