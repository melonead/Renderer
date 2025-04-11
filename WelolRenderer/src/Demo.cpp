#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Renderer.h"

#include "shader.h"

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

    glm::mat4 viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -90.0f));
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH /(float)SCR_HEIGHT, 0.1f, 100.0f);

    // --------------------------------------------------------------------
    
    /*

    // ------------------------- attributes
    
    Welol::VertexAttribute positionAttribute{ 0, Welol::WL_FLOAT3, vertexPositions.data(), 6, false};
    Welol::VertexAttribute colorAttribute{1, Welol::WL_FLOAT3, vertexColors.data(), 1, true};
    // --------------------------------------------


    // ----------------------------- Render Operation ---------------------------------

    Welol::RenderOperation triangleRenderOperation{ Welol::WL_TRIANGLES, numberOfRectVertices, 0, 0, false };
    triangleRenderOperation.addVertexAttribute(positionAttribute);
    triangleRenderOperation.addVertexAttribute(colorAttribute);
    // --------------------------------------------------------------------------------

        // -----------------------------------------------------------------------------------------------------------------------------------------
    // ----------- initialization ----------------------
    // initialize render operation
    GLRenderer.initializeRenderOperation(triangleRenderOperation);
    // activate render operation
    GLRenderer.activateRenderOperation(triangleRenderOperation);
    // activate attributes of the render operation
    GLRenderer.activateRenderOperationAttributes(triangleRenderOperation);
    // -----------------------------------------------------------------------------------------------------------------------------------------
    */





    // ----------------------------- Renderer -----------------------------------------
    Welol::Renderer GLRenderer;
    // --------------------------------------------------------------------------------







    // --------------------------------- instanced draw call demo ----------------------------------

    unsigned int numberOfInstances = 300;
    unsigned int numberOfRectVertices = 6;
    bool instanced = true;
    bool indexed = true;
    Welol::RenderOperation instancedRectangles{ Welol::WL_TRIANGLES, numberOfRectVertices, 0, numberOfInstances, instanced, indexed };

    // add vertex indices
    instancedRectangles.addVertexIndices(vertexIndices);
    
    // create displacement attribute for displacement of the rectangles
    std::vector<float> displacements;
    displacements.reserve(numberOfInstances * 2);

    float seperation = 2.0f;
    float xStart = -20.5f;
    float yStart = -30.25f;
    for (unsigned int y = 0; y < 30; y++)
    {
        float ySep = (float)y * seperation;
        float yPos = yStart + ySep;
        for (unsigned int x = 0; x < 10; x++)
        {
            float xSep = (float)x * seperation;
            float xPos = xStart + xSep;
            displacements.push_back(xPos);
            displacements.push_back(yPos);
        }
    }
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
    // activate render operation
    GLRenderer.activateRenderOperation(instancedRectangles);
    // activate attributes of the render operation
    GLRenderer.activateRenderOperationAttributes(instancedRectangles);

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

        /*
        // ----------- render loop --------------------------
        // activate current GPU program
        triangleShader.use();
        triangleShader.setMatrix4fv("viewMatrix", viewMatrix);
        triangleShader.setMatrix4fv("projectionMatrix", projectionMatrix);
        // activate render operation
        
        // openGL draw call
        //glDrawArrays(GL_TRIANGLES, triangleRenderOperation.getOffset(), triangleRenderOperation.getVertexCount());
        GLRenderer.render(triangleRenderOperation);
        // deactivate render operation
        */







        // ---------------------------------------------- instanced rects
        
        instancedRectsShader.use();
        instancedRectsShader.setMatrix4fv("viewMatrix", viewMatrix);
        instancedRectsShader.setMatrix4fv("projectionMatrix", projectionMatrix);

        GLRenderer.render(instancedRectangles);

        // --------------------------------------------------------------






        

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
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