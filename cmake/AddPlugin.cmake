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

function(fsh_add_plugin)
  cmake_parse_arguments(PARSE_ARGV 0 FSH
  # Options:
  ""
  # Single-value arguments:
  "IS_SYNTH;PLUGIN_CODE"
  # Multi-value arguments:
  "")

  juce_add_plugin(${PROJECT_NAME}
    PRODUCT_NAME              "fsh_${PROJECT_NAME}"
    PLUGIN_NAME               "fsh :: ${PROJECT_NAME}"
    PLUGIN_CODE               ${FSH_PLUGIN_CODE}

    # NEEDS_MIDI_INPUT must be true if IS_SYNTH is true, or AU validation will fail:
    IS_SYNTH                  ${FSH_IS_SYNTH}
    NEEDS_MIDI_INPUT          ${FSH_IS_SYNTH}

    PLUGIN_MANUFACTURER_CODE  Fstk
    FORMATS                   VST3 LV2

    COMPANY_NAME              fshstk
    COMPANY_WEBSITE           https://docs.fshstk.com

    COPY_PLUGIN_AFTER_BUILD   TRUE
  )

  target_link_libraries(${PROJECT_NAME} PRIVATE fshlib)
endfunction()
