cmake_minimum_required(VERSION 3.22)

project(fishSuite
    VERSION     0.0.1
    LANGUAGES   CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(FISH_COMPANY_NAME fshstk)
set(FISH_MANUFACTURER_CODE Fstk)
set(FISH_PLUGIN_FORMATS VST Standalone)
set(FISH_COPY_PLUGINS TRUE)

include(Dependencies.cmake)

# VST_SDK_PATH is defined in Dependencies.cmake:
juce_set_vst2_sdk_path(${VST_SDK_PATH})

enable_testing()

add_subdirectory(fonts)
add_subdirectory(src)
