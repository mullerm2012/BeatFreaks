# Builds and tests only the DeckForge core (automation, project file,
# diagnostics) without the rest of Mixxx. Fast enough for every push:
#   cmake -S tools/deckforge_core -B build-core -G Ninja && cmake --build build-core
#   ctest --test-dir build-core --output-on-failure
cmake_minimum_required(VERSION 3.21)
project(deckforge_core CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC OFF)

get_filename_component(MIXXX_SRC "${CMAKE_CURRENT_LIST_DIR}/../../src" ABSOLUTE)

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(GTest REQUIRED)

add_library(deckforge_core STATIC
  ${MIXXX_SRC}/deckforge/automation/automationlane.cpp
  ${MIXXX_SRC}/deckforge/automation/automationrecorder.cpp
  ${MIXXX_SRC}/deckforge/diagnostics/hardwaretest.cpp
  ${MIXXX_SRC}/deckforge/diagnostics/dflog.cpp
  ${MIXXX_SRC}/deckforge/diagnostics/diagnosticreport.cpp
  ${MIXXX_SRC}/deckforge/project/dfproject.cpp
)
target_include_directories(deckforge_core PUBLIC ${MIXXX_SRC})
target_link_libraries(deckforge_core PUBLIC Qt6::Core)
if(MSVC)
  target_compile_options(deckforge_core PRIVATE /W4 /WX)
else()
  target_compile_options(deckforge_core PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

add_executable(deckforge_core_tests
  ${MIXXX_SRC}/test/deckforge/automationlane_test.cpp
  ${MIXXX_SRC}/test/deckforge/automationrecorder_test.cpp
  ${MIXXX_SRC}/test/deckforge/dfproject_test.cpp
  ${MIXXX_SRC}/test/deckforge/automation_edgecase_test.cpp
)
target_link_libraries(deckforge_core_tests PRIVATE deckforge_core GTest::gtest GTest::gtest_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(deckforge_core_tests)
