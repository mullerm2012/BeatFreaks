# Builds and tests only the BeatFreaks core (automation, project file,
# diagnostics) without the rest of Mixxx. Fast enough for every push:
#   cmake -S tools/beatfreaks_core -B build-core -G Ninja && cmake --build build-core
#   ctest --test-dir build-core --output-on-failure
cmake_minimum_required(VERSION 3.21)
project(beatfreaks_core CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC OFF)

get_filename_component(MIXXX_SRC "${CMAKE_CURRENT_LIST_DIR}/../../src" ABSOLUTE)

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(GTest REQUIRED)

add_library(beatfreaks_core STATIC
  ${MIXXX_SRC}/beatfreaks/automation/automationlane.cpp
  ${MIXXX_SRC}/beatfreaks/automation/automationrecorder.cpp
  ${MIXXX_SRC}/beatfreaks/diagnostics/hardwaretest.cpp
  ${MIXXX_SRC}/beatfreaks/diagnostics/bflog.cpp
  ${MIXXX_SRC}/beatfreaks/diagnostics/diagnosticreport.cpp
  ${MIXXX_SRC}/beatfreaks/project/bfproject.cpp
)
target_include_directories(beatfreaks_core PUBLIC ${MIXXX_SRC})
target_link_libraries(beatfreaks_core PUBLIC Qt6::Core)
if(MSVC)
  target_compile_options(beatfreaks_core PRIVATE /W4 /WX)
else()
  target_compile_options(beatfreaks_core PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

add_executable(beatfreaks_core_tests
  ${MIXXX_SRC}/test/beatfreaks/automationlane_test.cpp
  ${MIXXX_SRC}/test/beatfreaks/automationrecorder_test.cpp
  ${MIXXX_SRC}/test/beatfreaks/bfproject_test.cpp
  ${MIXXX_SRC}/test/beatfreaks/automation_edgecase_test.cpp
)
target_link_libraries(beatfreaks_core_tests PRIVATE beatfreaks_core GTest::gtest GTest::gtest_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(beatfreaks_core_tests)
