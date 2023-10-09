include_directories(${CMAKE_SOURCE_DIR}/libLogger)

add_executable(LoggerExample main.cpp)

target_link_libraries(LoggerExample libLogger)

enable_testing()
