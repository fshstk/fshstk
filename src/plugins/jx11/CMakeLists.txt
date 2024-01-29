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
  PluginEditor.cpp
  PluginProcessor.cpp
  PluginState.cpp
)
