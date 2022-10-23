################################################################################
# FetchContent
################################################################################

include(FetchContent)

################################################################################
# JUCE - AU/VST plugin framework
################################################################################

FetchContent_Declare(
  JUCE
  GIT_REPOSITORY https://github.com/juce-framework/JUCE
  GIT_TAG        7.0.2)
FetchContent_MakeAvailable(JUCE)

################################################################################
# KFR - DSP library
################################################################################

FetchContent_Declare(
  KFR
  GIT_REPOSITORY https://github.com/kfrlib/kfr
  GIT_TAG        4.3.0)
FetchContent_MakeAvailable(KFR)

################################################################################
# Catch2 - Library for unit testing
################################################################################

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v2.13.9)
FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)
include(Catch)
