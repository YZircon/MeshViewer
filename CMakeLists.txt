cmake_minimum_required(VERSION 3.24)
project(MeshViewer)

set(CMAKE_CXX_STANDARD 20)

set(GLFW_DIR 3rdparty/glfw-3.3.8/cmake)


find_package(GLFW REQUIRED)
find_package(OpenGL REQUIRED)


include_directories(${GLFW_INCLUDE_DIRS})
include_directories(3rdparty/glad/include)
include_directories(3rdparty/eigen-3.4.0)
include_directories(3rdparty/OBJ-Loader)
include_directories(3rdparty/stb)
include_directories(3rdparty/imgui)

add_executable(${PROJECT_NAME} ${GLAD_SOURCE}
        src/main.cpp
        src/Geometry.cpp
        src/Geometry.h
        src/Camera.cpp
        src/Camera.h
        src/Shader.cpp
        src/Shader.h
        src/Mesh.cpp
        src/Mesh.h
        src/Light.cpp
        src/Material.cpp
        3rdparty/glad/src/glad.c
        3rdparty/imgui/backends/imgui_impl_glfw.cpp
        3rdparty/imgui/backends/imgui_impl_opengl3.cpp
        3rdparty/imgui/imgui_tables.cpp
        3rdparty/imgui/imgui_widgets.cpp
        3rdparty/imgui/imgui_draw.cpp
        3rdparty/imgui/imgui.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${GLFW_LIBRARIES})

file(COPY ${GLFW_DLL} DESTINATION ${CMAKE_BINARY_DIR})