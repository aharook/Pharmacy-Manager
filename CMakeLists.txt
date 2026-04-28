cmake_minimum_required(VERSION 3.20)

project(Pharmacy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(pharmacy_core INTERFACE)
target_include_directories(pharmacy_core INTERFACE src)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/Infrastructure/*.cpp src/Console/*.cpp)

# Main application executable
add_executable(pharmacy src/main.cpp ${SOURCE_FILES})
target_link_libraries(pharmacy PRIVATE pharmacy_core)

enable_testing()

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS tests/*.cpp)

if(TEST_SOURCES)
  add_executable(pharmacy_tests ${TEST_SOURCES} ${SOURCE_FILES})
  target_link_libraries(pharmacy_tests PRIVATE pharmacy_core GTest::gtest_main)
  include(GoogleTest)
  gtest_discover_tests(pharmacy_tests)
endif()
