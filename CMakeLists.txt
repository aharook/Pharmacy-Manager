cmake_minimum_required(VERSION 3.22)

project(Pharmacy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(pharmacy_core INTERFACE)
target_include_directories(pharmacy_core INTERFACE src)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*.cpp)
list(REMOVE_ITEM SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

add_executable(pharmacy src/main.cpp ${SOURCE_FILES})
target_link_libraries(pharmacy PRIVATE pharmacy_core)

enable_testing()

file(GLOB TEST_SOURCES "tests/*.cpp")

if(TEST_SOURCES)
  include(FetchContent)

  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
    GIT_SHALLOW TRUE
  )

  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  set(BUILD_GMOCK ON CACHE BOOL "" FORCE)

  FetchContent_MakeAvailable(googletest)

  add_executable(pharmacy_tests ${TEST_SOURCES} ${SOURCE_FILES})

  target_link_libraries(pharmacy_tests PRIVATE
    pharmacy_core
    GTest::gtest_main
    GTest::gmock
  )

  include(GoogleTest)
  gtest_discover_tests(pharmacy_tests)
  add_test(NAME PharmacyTests COMMAND pharmacy_tests)
else()
  message(WARNING "No test sources found in tests/")
endif()
