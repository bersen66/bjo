cmake_minimum_required(VERSION 3.5)

project(TEMPLATE VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

add_compile_options(
        -Werror

        -Wall
        -Wextra
        -Wpedantic

        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wenum-compare
        -Wfloat-equal
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Woverloaded-virtual
        -Wsign-conversion
        -Wsign-promo
)

include_directories(include)

#Add app-sources there
add_library(
    APPLICATION
    src/adder/adder.cpp
)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} APPLICATION)

get_directory_property(IS_SUBPROJECT PARENT_DIRECTORY)
option(ENABLE_TESTING "Enable testing" ON)
option(ENABLE_BENCHMARKS "Enable benchmarks" ON)


if (NOT ENABLE_TESTING)
    message(STATUS "Testing disabled")
elseif (IS_SUBPROJECT)
    message(STATUS "Do not build tests for subproject")
else ()
    add_subdirectory(tests)
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
endif()
