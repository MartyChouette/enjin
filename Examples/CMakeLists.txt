cmake_minimum_required(VERSION 3.20)

# Example: Triangle
add_executable(ExampleTriangle
    Triangle/main.cpp
)

target_link_libraries(ExampleTriangle PRIVATE
    EnjinEngine
    EnjinCore
)

target_compile_features(ExampleTriangle PUBLIC cxx_std_20)
