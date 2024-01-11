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
  GIT_TAG        7.0.9)
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
# FMT - C++ String Formatting
################################################################################

FetchContent_Declare(
  FMT
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        10.1.1
)
FetchContent_MakeAvailable(FMT)

################################################################################
# spdlog - C++ Logging Library
################################################################################

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog
  GIT_TAG        v1.12.0
)
set(SPDLOG_FMT_EXTERNAL ON)
FetchContent_MakeAvailable(spdlog)
