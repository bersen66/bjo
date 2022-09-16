cmake_minimum_required(VERSION 3.5)
project(TEMPLATE_BENCHMARKS)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} APPLICATION)

find_package(benchmark REQUIRED)
if (benchmark_FOUND)
    message("${PROJECT_NAME}: GOOGLE BENCHMARKS FOUND")
    include_directories(${benchmark_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${benchmark_LIBRARIES})
endif ()