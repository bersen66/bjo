cmake_minimum_required(VERSION 3.5)
project(${PROJECT_NAME}_CLIENT_EXAMPLE)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} core)

find_package(Boost REQUIRED)
if (Boost_FOUND)
    message("TESTS: Boost FOUND")
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
endif()

find_package(spdlog REQUIRED)
if (spdlog_FOUND)
    message("TESTS: spdlog  FOUND")
    include_directories(${spdlog_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${spdlog_LIBRARIES})
endif()

find_package(fmt REQUIRED)
if (fmt_FOUND)
    message("${PROJECT_NAME}: FOUND FMT")
    include_directories(${fmt_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${fmt_LIBRARIES})
endif()


find_package(ctre REQUIRED)
if (ctre_FOUND)
    message("${PROJECT_NAME}: FOUND CTRE")
    include_directories(${ctre_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${ctre_LIBRARIES})
endif()