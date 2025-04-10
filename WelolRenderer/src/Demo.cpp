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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,

        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f
    };

    std::vector<float> vertexColors = {

        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f
    };
    // ---------------------- matrices ------------------------------------

    glm::mat4 viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH /(float)SCR_HEIGHT, 0.1f, 100.0f);

    // --------------------------------------------------------------------


    // ------------------------- attributes
    
    Welol::VertexAttribute positionAttribute{ 0, Welol::WL_FLOAT3, vertexPositions.data(), 6};
    Welol::VertexAttribute colorAttribute{1, Welol::WL_FLOAT3, vertexColors.data(), 6};
    // --------------------------------------------


    // ----------------------------- Render Operation ---------------------------------
    Welol::RenderOperation triangleRenderOperation{ Welol::WL_TRIANGLES, (unsigned int) vertexPositions.size(), 0, 0, false };
    triangleRenderOperation.addVertexAttribute(positionAttribute);
    triangleRenderOperation.addVertexAttribute(colorAttribute);
    // --------------------------------------------------------------------------------


    // ----------------------------- Renderer -----------------------------------------
    Welol::Renderer GLRenderer;
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
    // 
    // GPU PROGRAM

    std::string vertexShaderPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\rectVertex.glsl";
    std::string fragmentShaderPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\rectFragment.glsl";
    
    Shader triangleShader{ vertexShaderPath, fragmentShaderPath };
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);


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