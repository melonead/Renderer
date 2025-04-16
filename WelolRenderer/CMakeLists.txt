cmake_minimum_required(VERSION 3.31)
set(CMAKE_CXX_STANDARD_REQUIED ON)
set(CMAKE_CXX_STANDARD 20)

project(RendererDemo VERSION 1.0)
add_executable(RendererDemo src/Demo.cpp src/glad.c src/Renderer.cpp src/shader.cpp src/Camera.cpp src/SceneView.cpp)
target_include_directories(RendererDemo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(RendererDemo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/ThirdParty)

find_library(GLFW3 glfw3 REQUIRED PATH lib)
target_link_libraries(RendererDemo PUBLIC ${GLFW3})