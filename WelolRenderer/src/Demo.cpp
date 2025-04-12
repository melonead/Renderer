#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Renderer.h"

#include "shader.h"
#include "Camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Welol Renderer Demo", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::vector<float> vertexPositions = {

        0.5f,  0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
       -0.5f, -0.5f, 0.0f, // bottom left
       -0.5f,  0.5f, 0.0f // top left

    };

    std::vector<unsigned int> vertexIndices = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    std::vector<float> vertexColors = {

        0.0f, 1.0f, 0.0f,
  
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f

    };


    // ---------------------- matrices ------------------------------------
    Welol::Camera camera{glm::vec3(0.0f, 0.0f, 60.0f), glm::vec3(0.0f, 0.0f, 0.0f)};
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH /(float)SCR_HEIGHT, 0.1f, 1000.0f);

    // --------------------------------------------------------------------
  

    // ----------------------------- Renderer -----------------------------------------
    Welol::Renderer GLRenderer;
    // --------------------------------------------------------------------------------

    // --------------------------------- instanced draw call demo ----------------------------------

    
    unsigned int numberOfInstances = 10000;
    unsigned int numberOfRectVertices = 6;
    bool instanced = true;
    bool indexed = true;

    // create displacement attribute for displacement of the rectangles
    std::vector<float> displacements;
    displacements.reserve(numberOfInstances * 2);

    float seperation = 2.0f;
    float xStart = -50.5f;
    float yStart = -50.25f;
    for (unsigned int y = 0; y < 100; y++)
    {
        float ySep = (float)y * seperation;
        float yPos = yStart + ySep;
        for (unsigned int x = 0; x < 100; x++)
        {
            float xSep = (float)x * seperation;
            float xPos = xStart + xSep;
            displacements.push_back(xPos);
            displacements.push_back(yPos);
        }
    }







    Welol::RenderOperation instancedRectangles{ Welol::WL_TRIANGLES, numberOfRectVertices, 0, numberOfInstances, instanced, indexed };

    // add vertex indices
    instancedRectangles.addVertexIndices(vertexIndices);
    
    // attach vertex position attributes (make it instanced)
    Welol::VertexAttribute position{ 0, Welol::WL_FLOAT3, vertexPositions.data(), 6, false };
    // attach vertex color attributes (make it instanced)
    Welol::VertexAttribute color{ 1, Welol::WL_FLOAT3, vertexColors.data(), 6, false };
    // attach displacement attributes (NOT instanced)
    Welol::VertexAttribute disp{ 2, Welol::WL_FLOAT2, displacements.data(), numberOfInstances, true };

    // add the attributes to the render operation
    instancedRectangles.addVertexAttribute(position);
    instancedRectangles.addVertexAttribute(color);
    instancedRectangles.addVertexAttribute(disp);

    // ----------- initialization ----------------------
    // initialize render operation
    GLRenderer.initializeRenderOperation(instancedRectangles);
    // ---------------------------------------------------------------------------------------------



    // 
    // GPU PROGRAM

    std::string vertexShaderPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\rectVertex.glsl";
    std::string fragmentShaderPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\rectFragment.glsl";
    
    Shader triangleShader{ vertexShaderPath, fragmentShaderPath };


    std::string instanceRectsVertexShaderPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\instancedRectsVertex.glsl";
    std::string instanceRectsFragmentShaderPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\instancedRectsFragment.glsl";
    Shader instancedRectsShader{ instanceRectsVertexShaderPath , instanceRectsFragmentShaderPath };

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ---------------------------------------------- instanced rects
        
        glm::mat4 mat = camera.getViewMatrix();
        instancedRectsShader.use();
        instancedRectsShader.setMatrix4fv("viewMatrix", camera.getViewMatrix());
        instancedRectsShader.setMatrix4fv("projectionMatrix", projectionMatrix);

        GLRenderer.render(instancedRectangles);
        

        camera.forwardBy(-2.5f);
        camera.update();
        // --------------------------------------------------------------



        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    vertexPositions.clear();
    vertexColors.clear();
    vertexIndices.clear();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}