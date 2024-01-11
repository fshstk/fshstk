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

project(FishCoder
  VERSION     ${CMAKE_PROJECT_VERSION}
  LANGUAGES   CXX)

juce_add_plugin(${PROJECT_NAME}
  PLUGIN_CODE               StEn
  IS_SYNTH                  FALSE
  NEEDS_MIDI_INPUT          FALSE
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
  PluginEditor.cpp
  PluginProcessor.cpp
  PluginState.cpp
)

add_subdirectory(sections)
