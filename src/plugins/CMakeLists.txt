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

set(FSH_MANUFACTURER_CODE Fstk)

if(APPLE OR WIN32)
  set(FSH_PLUGIN_FORMATS VST VST3 LV2)
else()
  # TODO: Linux segfaults when linking LV2 plugin, so we disable it for now:
  set(FSH_PLUGIN_FORMATS VST VST3)
endif()

set_directory_properties(PROPERTIES
  JUCE_COMPANY_NAME               fshstk
  JUCE_COMPANY_WEBSITE            https://docs.fshstk.com
)

# Development is done on macOS, for now we don't really care about other OSes:
if(APPLE)
set_directory_properties(PROPERTIES
  JUCE_COPY_PLUGIN_AFTER_BUILD    TRUE
  JUCE_LV2_COPY_DIR               "$ENV{HOME}/Library/Audio/Plug-Ins/LV2"
  JUCE_VST_COPY_DIR               "$ENV{HOME}/Library/Audio/Plug-Ins/VST/fshstk"
  JUCE_VST3_COPY_DIR              "$ENV{HOME}/Library/Audio/Plug-Ins/VST3/fshstk"
)
endif()

add_subdirectory(encoder)
add_subdirectory(synth1)
add_subdirectory(jx11)
