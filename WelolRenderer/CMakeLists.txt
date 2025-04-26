cmake_minimum_required(VERSION 3.31)
set(CMAKE_CXX_STANDARD_REQUIED ON)
set(CMAKE_CXX_STANDARD 20)

project(Renderer VERSION 1.0)

# First Demo
add_executable(
    RendererDemo 
    src/Demo.cpp 
    src/glad.c 
    src/Renderer.cpp 
    src/shader.cpp 
    src/Camera.cpp 
    src/SceneView.cpp  
    src/string_funcs.cpp 
    src/objloader.cpp 
    src/WelolMath.cpp
    src/Texture.cpp
)

target_include_directories(RendererDemo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(RendererDemo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/ThirdParty)

target_link_libraries(RendererDemo PUBLIC ${GLFW3})

# Sky Box demo

add_executable(
    SkyBoxDemo 
    src/SkyBox.cpp 
    src/glad.c 
    src/Renderer.cpp 
    src/shader.cpp 
    src/Camera.cpp 
    src/SceneView.cpp  
    src/string_funcs.cpp 
    src/objloader.cpp 
    src/WelolMath.cpp
    src/Texture.cpp
)


target_include_directories(SkyBoxDemo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(SkyBoxDemo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/ThirdParty)
target_link_libraries(SkyBoxDemo PUBLIC ${GLFW3})

find_library(GLFW3 glfw3 REQUIRED PATH lib)
