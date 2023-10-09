include_directories(${CMAKE_SOURCE_DIR}/libLogger)

find_package(Catch2 2 QUIET)
if(Catch2_FOUND)
    message(STATUS "Packaged version of Catch will be used.")
else()
    message(STATUS "Bundled version of Catch will be downloaded and used.")
    include(FetchContent)
    FetchContent_Declare(Catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2.git GIT_TAG v2.13.10)
    FetchContent_MakeAvailable(Catch2)
endif()

add_executable(LoggerTest test.cpp)

target_link_libraries(LoggerTest libLogger Catch2::Catch2)

enable_testing()
