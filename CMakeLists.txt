cmake_minimum_required(VERSION 3.5)

project(core VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

include_directories(include)

#Add app-sources there
add_library(
    ${PROJECT_NAME}

     src/core/config.cpp
     src/core/connection.cpp
     src/core/handlers_map.cpp
     src/core/listener.cpp
     src/core/parsing_stuff.cpp
     src/core/router.cpp
     src/core/run_session.cpp
     src/core/server.cpp
     src/core/messages.cpp
     src/core/executor.cpp
)


get_directory_property(IS_SUBPROJECT PARENT_DIRECTORY)
option(ENABLE_TESTING "Enable testing" OFF)
option(ENABLE_EXAMPLES "Enable examples" ON)
option(ENABLE_BENCHMARKS "Enable benchmarks" OFF)


if (NOT ENABLE_TESTING)
    message(STATUS "Testing disabled")
elseif (IS_SUBPROJECT)
    message(STATUS "Do not build tests for subproject")
else ()
    add_subdirectory(tests)
endif ()


if (NOT ENABLE_EXAMPLES)
    message(STATUS "examples disabled")
elseif (IS_SUBPROJECT)
    message(STATUS "Do not build examples for subproject")
else ()
    add_subdirectory(examples)
endif ()

if (NOT ENABLE_BENCHMARKS)
    message(STATUS "Benchmarks disabled")
elseif (IS_SUBPROJECT)
    message(STATUS "Do not build benchmarks for subproject")
else ()
    add_subdirectory(benchmarks)
endif ()

find_package(fmt REQUIRED)
if (fmt_FOUND)
    message("${PROJECT_NAME}: FOUND FMT")
    include_directories(${fmt_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${fmt_LIBRARIES})
endif(fmt_FOUND)

find_package(Boost REQUIRED)
if (Boost_FOUND)
    message("${PROJECT_NAME}: BOOST FOUND!")
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
endif(Boost_FOUND)

find_package(spdlog REQUIRED)
if (spdlog_FOUND)
    message("${PROJECT_NAME}: SPDOLOG FOUND!")
    include_directories(${spdlog_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${spdlog_LIBRARIES})
else ()
    message("${PROJECT_NAME}: SPDLOG NOT FOUND!")
endif (spdlog_FOUND)

find_package(jsoncpp REQUIRED)
if (jsoncpp_FOUND)
    message("${PROJECT_NAME}: JSONCPP FOUND!")
    include_directories(${jsoncpp_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${jsoncpp_LIBRARIES})
else ()
    message("${PROJECT_NAME}: JSONCPP NOT FOUND")
endif (jsoncpp_FOUND)

find_package(yaml-cpp REQUIRED)
if (yaml-cpp_FOUND)
    message("${PROJECT_NAME}: YAML-CPP FOUND")
    include_directories(${yaml-cpp_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${yaml-cpp_LIBRARIES})
else ()
    message("${PROJECT_NAME}: YAML-CPP NOT FOUND")
endif (yaml-cpp_FOUND)

find_package(ctre REQUIRED)
if (ctre_FOUND)
    message("${PROJECT_NAME}: FOUND CTRE")
    include_directories(${ctre_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${ctre_LIBRARIES})
else ()
    message("${PROJECT_NAME}: CTRE NOT FOUND")
endif()