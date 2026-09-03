cmake_minimum_required(VERSION 3.15)
project(EngineV2 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch GLFW from GitHub
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# Find all source and header files (avoiding the 'build' directory)
file(GLOB ROOT_SOURCES CONFIGURE_DEPENDS "*.cpp" "*.hpp" "*.h")
file(GLOB_RECURSE SRC_SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.hpp" "src/*.h")
set(SOURCES ${ROOT_SOURCES} ${SRC_SOURCES} "src/glad/glad.c")

add_executable(EngineV2 ${SOURCES})
target_link_libraries(EngineV2 PRIVATE glfw)

# For macOS explicit OpenGL linkage
if(APPLE)
    find_library(OPENGL_LIBRARY OpenGL)
    target_link_libraries(EngineV2 PRIVATE ${OPENGL_LIBRARY})
endif()
