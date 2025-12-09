set(testedSourceDir ${CMAKE_SOURCE_DIR}/src)

set(TEST_RUNNER_NAME ReaderTest)

add_executable(${TEST_RUNNER_NAME})
target_sources(${TEST_RUNNER_NAME} PRIVATE ./main.cpp ./ReaderTest.cpp
                                           ./MockReader.cpp)

target_include_directories(${TEST_RUNNER_NAME} PUBLIC ../include/)

target_link_libraries(${TEST_RUNNER_NAME} PRIVATE libstream gtest)

add_test(NAME ${TEST_RUNNER_NAME} COMMAND ${TEST_RUNNER_NAME})
set_tests_properties(${TEST_RUNNER_NAME} PROPERTIES DEPENDS bitcastTest)
