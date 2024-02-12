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

cmake_minimum_required(VERSION 3.22)

project(fshstk
    VERSION     0.0.1
    LANGUAGES   CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if(UNIX AND NOT APPLE) # (if Linux)
# Required by spdlog:
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
endif()

if(APPLE)
# Build universal binaries on Mac:
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()

include(cmake/Dependencies.cmake)

enable_testing()

find_package(Doxygen)
if (DOXYGEN_FOUND)
  add_custom_target(docs
    COMMAND             ${DOXYGEN_EXECUTABLE}
    WORKING_DIRECTORY   ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT             "Generating docs with doxygen"
    VERBATIM)
endif()

add_subdirectory(assets)
add_subdirectory(src)
