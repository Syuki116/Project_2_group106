set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_minimum_required(VERSION 3.29)
project(Project_2_group106)

# Course toolchain
#set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Warnings
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

include_directories(src)

# Catch2 via FetchContent
include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.5.2
)
FetchContent_MakeAvailable(Catch2)

# Main executable (NO test sources here)
add_executable(app
        src/main.cpp
        src/grid_index.cpp src/grid_index.h
        src/kd_tree.cpp    src/kd_tree.h
        src/load_csv.cpp   src/load_csv.h
        src/trip.h
)

# Tests executable
add_executable(tests
        test/tests.cpp
        src/grid_index.cpp src/grid_index.h
        src/kd_tree.cpp    src/kd_tree.h
        src/load_csv.cpp   src/load_csv.h
        src/trip.h
)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)