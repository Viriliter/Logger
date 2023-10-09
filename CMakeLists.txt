cmake_minimum_required(VERSION 3.7)
project(Logger VERSION 0.1.0 LANGUAGES C CXX)

set(BUILD_LOGGER_TEST ON)
set(BUILD_LOGGER_EXAMPLE ON)

include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(logger)

if(BUILD_LOGGER_EXAMPLE)
    add_subdirectory(example)
endif(BUILD_LOGGER_EXAMPLE)

if(BUILD_LOGGER_TEST)
    add_subdirectory(test)

    enable_testing()

    add_test(NAME LoggerTestCases COMMAND LoggerTest)
endif(BUILD_LOGGER_TEST)
