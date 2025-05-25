cmake_minimum_required(VERSION 3.0...3.28.0)

set(PROJECT_NAME TEMPLA)

project(${PROJECT_NAME} VERSION 0.0.1 DESCRIPTION "TEMPLA : C++ metaprogramming utilities" LANGUAGES CXX)

message("INFO :")
message("  C: '${CMAKE_C_COMPILER}'")
message("  C++: '${CMAKE_CXX_COMPILER}'")
message("  Source: ${PROJECT_SOURCE_DIR}")
message("  Binary: ${PROJECT_BINARY_DIR}")
message("  Version: ${PROJECT_VERSION}")
message("  Version (alt): ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define the executable
add_executable(${PROJECT_NAME} 
    main.cpp
)

message("Processing CMakeLists.txt")