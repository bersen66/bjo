cmake_minimum_required(VERSION 3.5)
project(TEMPALATE_TESTS)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} APPLICATION)

find_package(GTest REQUIRED)
if (GTest_FOUND)
    message("TESTS: GTEST FOUND")
    include_directories(${GTest_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${GTest_LIBRARIES})
endif()