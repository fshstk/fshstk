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

# In order to build VST2 plug-ins, we need the VST2 SDK which was removed at
# some point in Juce 5.
# Here we grab the last commit before the SDK was removed and make it available
# via the VST_SDK_PATH variable:
FetchContent_Declare(
  JUCE_VST
  GIT_REPOSITORY https://github.com/juce-framework/JUCE
  GIT_TAG        a54535bc317b5c005a8cda5c22a9c8d4c6e0c67e)
FetchContent_GetProperties(JUCE_VST)
if(NOT juce_vst_POPULATED)
  FetchContent_Populate(JUCE_VST)
endif()
set(VST_SDK_PATH
  ${juce_vst_SOURCE_DIR}/modules/juce_audio_processors/format_types/VST3_SDK)

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

################################################################################
# Boost - All-purpose library that includes spherical harmonics
################################################################################

# FetchContent_Declare(
#   Boost
#   GIT_REPOSITORY https://github.com/boostorg/boost.git
#   GIT_TAG        boost-1.80.0
# )
# FetchContent_MakeAvailable(Boost)

################################################################################
# FMT - C++ String Formatting
################################################################################

FetchContent_Declare(
  FMT
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        9.1.0
)
FetchContent_MakeAvailable(FMT)
