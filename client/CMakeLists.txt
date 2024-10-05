cmake_minimum_required(VERSION 3.14)

# Project name
project(wdywclient)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG "master"
  GIT_PROGRESS TRUE
)

# Fetch and add raylib
FetchContent_MakeAvailable(raylib)

FetchContent_Declare(
  raygui
  GIT_REPOSITORY https://github.com/raysan5/raygui.git
  GIT_TAG "master"
  GIT_PROGRESS TRUE
)

# Fetch and add raygui
FetchContent_MakeAvailable(raygui)

# Check if building with Emscripten
if(${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_GLFW=3 -s ASYNCIFY -s USE_WEBGL2=1")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

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

# Add the source files
add_executable(wdywclient src/main.cpp
        src/stuff.h
        src/stuff.cpp)

target_include_directories(wdywclient PRIVATE ${raygui_SOURCE_DIR}/src)

# Link raylib to your executable
target_link_libraries(wdywclient PRIVATE raylib httplib::httplib nlohmann_json::nlohmann_json)

# Emscripten-specific settings
# if(${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
#     #set_target_properties(wdywclient PROPERTIES
#     #LINK_FLAGS "--shell-file ${CMAKE_SOURCE_DIR}/shell.html"
#   )
# endif()
