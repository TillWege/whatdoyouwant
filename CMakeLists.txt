# Top-level CMakeLists.txt
cmake_minimum_required(VERSION 3.14)
project(wdyw_combined)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define the subdirectories for client and server
add_subdirectory(server)
add_subdirectory(client)
