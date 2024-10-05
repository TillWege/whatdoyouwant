# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(wdywserver)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG "master"
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(httplib)

FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG "master"
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(json)

# Add executable
add_executable(wdywserver src/main.cpp)

# Link httplib to the target
target_link_libraries(wdywserver PRIVATE httplib::httplib nlohmann_json::nlohmann_json)
