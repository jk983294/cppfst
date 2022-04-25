cmake_minimum_required(VERSION 3.15)
project(external_demo)

set(CMAKE_CXX_STANDARD 17)

# use OpenMP if available
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
else()
    message("No OpenMP detected, fstlib builds without OpenMP but needs it for optimal performance!")
endif()

include_directories(/opt/version/latest/cppfst/include)

link_directories(/opt/version/latest/cppfst/lib)

add_executable(demo main.cpp)
target_link_libraries( demo fst libzstd liblz4 pthread rt z dl )