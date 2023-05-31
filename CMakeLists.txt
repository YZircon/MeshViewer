cmake_minimum_required(VERSION 3.24)
project(MeshViewer)

set(CMAKE_CXX_STANDARD 20)

set(GLAD_DIR 3rdparty/glad/cmake)
set(GLFW_DIR 3rdparty/GLFW/cmake)
set(Eigen_DIR 3rdparty/Eigen/cmake)

find_package(GLAD REQUIRED)
find_package(GLFW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Eigen REQUIRED)

include_directories(${GLAD_INCLUDE_DIRS})
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${Eigen_INCLUDE_DIRS})
include_directories(3rdparty/OBJLoader)
include_directories(3rdparty/stb)

add_executable(${PROJECT_NAME} ${GLAD_SOURCE}
        src/main.cpp
        src/Geometry.cpp
        src/Geometry.h
        src/Camera.cpp
        src/Camera.h
        src/Shader.cpp
        src/Shader.h
        src/Mesh.cpp
        src/Mesh.h src/Light.cpp src/Material.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${GLFW_LIBRARIES})

file(COPY ${GLFW_DLL} DESTINATION ${CMAKE_BINARY_DIR})