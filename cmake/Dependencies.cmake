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

include(FetchContent)

####################################################################################################
# JUCE - AU/VST plugin framework
####################################################################################################

message(STATUS "Fetching JUCE...")
FetchContent_Declare(
  JUCE
  GIT_REPOSITORY https://github.com/juce-framework/JUCE
  GIT_TAG        7.0.9)
FetchContent_MakeAvailable(JUCE)

# In order to build VST2 plug-ins, we need the VST2 SDK which was removed at some point in Juce 5.
# Here we grab the last commit before the SDK was removed and make it available via the VST_SDK_PATH
# variable:
message(STATUS "Fetching VST2 SDK...")
FetchContent_Declare(
  JUCE_VST
  GIT_REPOSITORY https://github.com/juce-framework/JUCE
  GIT_TAG        a54535bc317b5c005a8cda5c22a9c8d4c6e0c67e)
FetchContent_GetProperties(JUCE_VST)
if(NOT juce_vst_POPULATED)
  FetchContent_Populate(JUCE_VST)
endif()
set(VST_SDK_PATH ${juce_vst_SOURCE_DIR}/modules/juce_audio_processors/format_types/VST3_SDK)

####################################################################################################
# FMT - C++ String Formatting
####################################################################################################

message(STATUS "Fetching FMT...")
FetchContent_Declare(
  FMT
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        10.1.1
)
FetchContent_MakeAvailable(FMT)

####################################################################################################
# spdlog - C++ Logging Library
####################################################################################################

message(STATUS "Fetching spdlog...")
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog
  GIT_TAG        v1.12.0
)
set(SPDLOG_FMT_EXTERNAL ON)
FetchContent_MakeAvailable(spdlog)

####################################################################################################
# doxygen-awesome-css - Custom CSS theme for doxygen
####################################################################################################

message(STATUS "Fetching doxygen-awesome-css...")
FetchContent_Declare(
  doxygen-css
  GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css
  GIT_TAG        v2.3.1
)
FetchContent_MakeAvailable(doxygen-css)
