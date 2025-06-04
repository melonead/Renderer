cmake_minimum_required(VERSION 4.0) 
set(CMAKE_CXX_STANDARD_REQUIED ON)
set(CMAKE_CXX_STANDARD 20)

project(Renderer VERSION 1.0)

find_library(GLFW3  glfw3 REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/lib)
find_library(FTP  freetype REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# create imgui lib

add_library(mImGui STATIC
    imgui/imgui.cpp
    imgui/imgui_impl_opengl3.cpp
    imgui/imgui_impl_glfw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
)

target_include_directories(mImGui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
target_include_directories(mImGui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/ThirdParty)

# First Demo
add_executable(
    RendererDemo 

    FirstDemo/Demo.cpp 
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

    SkyBox/SkyBox.cpp 
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


# Fluid Simulation
add_subdirectory(FluidSim)

# Lighting
add_subdirectory(LightingDemo)

find_library(ft freetype PATHS "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\lib")
target_include_directories(RendererDemo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(RendererDemo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/ThirdParty)
# target_include_directories(ft PUBLIC PRIVATE )

target_link_libraries(SphSimulation PUBLIC ${FTP})


add_executable(
    ComputeOne

    ComputeShadersLearning/ComputeOne.cpp
    src/glad.c 
    src/Camera.cpp
    src/shader.cpp 
    src/WelolMath.cpp
    src/Renderer.cpp
)

target_include_directories(ComputeOne PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(ComputeOne PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/ThirdParty)
target_link_libraries(ComputeOne PUBLIC ${GLFW3})


find_package(OpenMP REQUIRED)
if (OpenMP_CXX_FOUND)
    target_link_libraries(SphSimulation PUBLIC OpenMP::OpenMP_CXX)
endif()

 
