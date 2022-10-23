add_executable(UnitTests TestMain.cpp)

target_sources(UnitTests PRIVATE
  DemoTestCase.cpp
)

target_link_libraries(UnitTests PRIVATE ${CMAKE_PROJECT_NAME} Catch2::Catch2 LightOrgan)
catch_discover_tests(UnitTests)
