set(TEST_RUNNER_NAME BitReaderTest)

add_executable(${TEST_RUNNER_NAME})
target_sources(${TEST_RUNNER_NAME} PRIVATE ./main.cpp ./BitReaderTest.cpp
                                           ../common/MockReader.cpp)

target_include_directories(${TEST_RUNNER_NAME} PUBLIC ../common/)

target_compile_options(${TEST_RUNNER_NAME} PRIVATE -Wall -Wextra -pedantic -g3
                                                   -fdiagnostics-color=always)

target_link_libraries(${TEST_RUNNER_NAME} PRIVATE libstream gtest)

add_test(NAME ${TEST_RUNNER_NAME} COMMAND ${TEST_RUNNER_NAME})
set_tests_properties(${TEST_RUNNER_NAME} PROPERTIES DEPENDS bitcastTest)
set_tests_properties(${TEST_RUNNER_NAME} PROPERTIES DEPENDS ReaderTest)
