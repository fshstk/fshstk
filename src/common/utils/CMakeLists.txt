target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(${PROJECT_NAME} PRIVATE
  AmbisonicEncoder.cpp
  EnvelopeFollower.cpp
  FeedbackDelayNetwork.cpp
  SphericalHarmonics.cpp
)
