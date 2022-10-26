target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
  Main.cpp
  PluginEditor.cpp
  PluginProcessor.cpp
  PluginState.cpp
  utils/efficientSHvanilla.cpp
  utils/Quaternion.cpp
  utils/YawPitchRoll.cpp
  utils/SphericalVector.cpp
)
