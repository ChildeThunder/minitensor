cmake_minimum_required(VERSION 3.16)
PROJECT(MiniTensor)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()
add_library(minitensor STATIC
    src/tensor.cpp
    src/allocator.cpp
)

target_include_directories(minitensor
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
add_executable(minitensor_demo
    examples/main.cpp
)

target_link_libraries(minitensor_demo
    PRIVATE
        minitensor
)
add_subdirectory(tests)
add_executable(bench_add
    examples/bench_add.cpp
)

target_link_libraries(bench_add
    PRIVATE minitensor
)
add_executable(debug_lab
    examples/debug_lab.cpp
)