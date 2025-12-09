cmake_minimum_required(VERSION 3.16)

project(libstream VERSION 0.1.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

option(LIBSTREAM_BUILD_TESTS "Build and run tests using CTest" ON)

if(NOT TARGET bitcast)
  message(
    STATUS "---- No libbitcast here or in parent project: fetching libbitcast")

  include(FetchContent)
  FetchContent_Declare(
    bitcast_dep
    GIT_REPOSITORY https://github.com/vincent-lafouasse/libbitcast.git
    GIT_TAG "main"
    GIT_SHALLOW 1)
  FetchContent_MakeAvailable(bitcast_dep)
else()
  message(STATUS "---- Found libbitcast in parent project: no fetching")
endif()

add_library(${PROJECT_NAME})
target_sources(
  ${PROJECT_NAME}
  PRIVATE src/Reader.c
  PRIVATE src/BitReader.c
  PUBLIC FILE_SET
         myHeaders
         TYPE
         HEADERS
         BASE_DIRS
         include
         FILES
         include/libstream.h
         include/libstream/Reader.h
         include/libstream/BitReader.h)
target_link_libraries(${PROJECT_NAME} PRIVATE bitcast)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic
                                               -fdiagnostics-color=always)

if(LIBSTREAM_BUILD_TESTS)
  include(FetchContent)
  include(cmake/fetch_gtest.cmake)

  include(CTest)
  enable_testing()

  add_subdirectory(test)
endif()
