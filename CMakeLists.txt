cmake_minimum_required(VERSION 3.15)
project(EngineV2 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

# Fetch Dear ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
)
FetchContent_MakeAvailable(imgui)

# Fetch GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG master
)
FetchContent_GetProperties(glm)
if(NOT glm_POPULATED)
    FetchContent_Populate(glm)
endif()

# Get path to ImGui and GLM
FetchContent_GetProperties(imgui SOURCE_DIR IMGUI_DIR)
FetchContent_GetProperties(glm SOURCE_DIR GLM_DIR)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Find all source and header files (avoiding the 'build' directory)
file(GLOB ROOT_SOURCES CONFIGURE_DEPENDS "*.cpp" "*.hpp" "*.h")
file(GLOB_RECURSE SRC_SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.hpp" "src/*.h")
set(SOURCES ${ROOT_SOURCES} ${SRC_SOURCES} "src/glad/glad.c" ${IMGUI_SOURCES})

add_executable(EngineV2 ${SOURCES})
target_include_directories(EngineV2 PRIVATE ${IMGUI_DIR} ${IMGUI_DIR}/backends ${GLM_DIR})
target_link_libraries(EngineV2 PRIVATE glfw)

# For macOS explicit OpenGL linkage
if(APPLE)
    find_library(OPENGL_LIBRARY OpenGL)
    target_link_libraries(EngineV2 PRIVATE ${OPENGL_LIBRARY})
endif()
