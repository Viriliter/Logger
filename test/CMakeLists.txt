include_directories(${CMAKE_SOURCE_DIR}/libLogger)

find_package(Catch2 REQUIRED)

add_executable(LoggerTest test.cpp)

target_link_libraries(LoggerTest libLogger Catch2::Catch2)

enable_testing()
