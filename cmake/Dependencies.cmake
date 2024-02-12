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
