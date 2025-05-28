#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Renderer.h"

#include "shader.h"
#include "Camera.h"
#include "SceneView.h"
#include "objloader.h"
#include "Texture.h"
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, bool& rotateCamera);
void mouse_callback(GLFWwindow* window,double xpos,double ypos);
void mouse_scroll_callback(GLFWwindow*window, double x, double y);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
bool rotFlag = false;
/*
    mouse input 
*/

double lastX = 0.0;
double lastY = 0.0;
float pitch = 0.0f;
float yaw = 0.0f;
bool firstMouse = true;
bool forwardCamera = false;
float cameraScrollDirection = 1.0f;

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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // ---------------------- matrices ------------------------------------

    // REVISIT: Why can we not have camera positioned only at the x axis?
    // For example the initial camera position cannot be (0.0f, 30.0f, 0.0f). 
    Welol::Camera* camera = new Welol::Camera(glm::vec3(30.0f, 30.0f, 40.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    float focusAngle = 45.0f;
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(focusAngle), (float)SCR_WIDTH /(float)SCR_HEIGHT, nearPlane, farPlane);
    // --------------------------------------------------------------------


    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);

    //stbi_set_flip_vertically_on_load(true);
    // 
    // GPU PROGRAM

    // -------------------------- scene view





    std::string cameraTexturePath1 = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\skybox\\cubemap\\cubemap_posx.png";
    std::string cameraTexturePath2 = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\skybox\\cubemap\\cubemap_posy.png";
    std::string cameraTexturePath3 = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\skybox\\cubemap\\cubemap_posz.png";
    std::string cameraTexturePath4 = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\skybox\\cubemap\\cubemap_negx.png";
    std::string cameraTexturePath5 = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\skybox\\cubemap\\cubemap_negy.png";
    std::string cameraTexturePath6 = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\skybox\\cubemap\\cubemap_negz.png";

    
    std::string cameraTextureName = "sky";
    unsigned int texUnit = 0;
    Welol::Texture cameraTexture{Welol::WL_TEX_CUBE_MAP, cameraTexturePath3, 0, cameraTextureName, texUnit};
    cameraTexture.addFaceTextureToCubeMap(Welol::WL_CBM_PX, cameraTexturePath1);
    cameraTexture.addFaceTextureToCubeMap(Welol::WL_CBM_PY, cameraTexturePath2);
    cameraTexture.addFaceTextureToCubeMap(Welol::WL_CBM_PZ, cameraTexturePath3);
    cameraTexture.addFaceTextureToCubeMap(Welol::WL_CBM_NX, cameraTexturePath4);
    cameraTexture.addFaceTextureToCubeMap(Welol::WL_CBM_NY, cameraTexturePath5);
    cameraTexture.addFaceTextureToCubeMap(Welol::WL_CBM_NZ, cameraTexturePath6);

    


    std::vector<glm::vec4> clipSpaceSkyBoxCoordinates = {
        glm::vec4(-1.0f,  3.0f, 0.999, 1.0f),
        glm::vec4(-1.0f, -1.0f, 0.999, 1.0f),
        glm::vec4( 3.0f, -1.0f, 0.999, 1.0f)
    };

    std::vector<glm::vec4> worldSpaceSkyBoxDirections = {
        glm::vec4(-1.0f,  3.0f, 0.999, 1.0f),
        glm::vec4(-1.0f, -1.0f, 0.999, 1.0f),
        glm::vec4( 3.0f, -1.0f, 0.999, 1.0f)
    };

    std::vector<glm::vec3> worldSpaceSkyBoxPositions = {
        glm::vec3(-16.0f,  7.0f,  -10.999),
        glm::vec3(-16.0f,  4.0f,  10.999),
        glm::vec3(-14.0f,  -10.0f,  10.999)
    };

    glm::mat4x4 inverseViewProjection(1.0f);

    Welol::Renderer GLRenderer;

    Welol::RenderOperation skyBoxRenderOperation{Welol::WL_TRIANGLES, 3, 0, 0, false, false};
    Welol::VertexAttribute positions{0, Welol::WL_FLOAT3, worldSpaceSkyBoxPositions.data(), 3, false};
    Welol::VertexAttribute directions{1, Welol::WL_FLOAT4, worldSpaceSkyBoxDirections.data(), 3, false};
    skyBoxRenderOperation.addVertexAttribute(positions);
    skyBoxRenderOperation.addVertexAttribute(directions);
    GLRenderer.initializeRenderOperation(skyBoxRenderOperation);

    std::string skyVertexShaderPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\shaders\\skyVertex.glsl";
    std::string skyFragmentShaderPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\shaders\\skyFragment.glsl";
    Shader skyBoxShader{ skyVertexShaderPath , skyFragmentShaderPath };




    float axisLength = 80.0f;
    Welol::SceneView sceneView{axisLength, GLRenderer};
    // -------------------------------------

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    bool rotateCamera = false;
    
    float cameraRotYaw = 0.0f;
    float cameraRotPitch = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window, rotateCamera);
        glClearColor(0.24705882352941178f, 0.24705882352941178f, 0.24705882352941178f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        


        // ---------------------------------------------- instanced rects

        glm::mat4 view = camera->getViewMatrix();
        sceneView.update(GLRenderer, projectionMatrix, view);
        
        inverseViewProjection = glm::inverse(projectionMatrix * view);
        
        if (!rotateCamera)
        {
            yaw = 0.0f;
            pitch = 0.0f;
        }
        
        if (rotateCamera)
        {
            camera->update(cameraScrollDirection, yaw, pitch, lastX, lastY, forwardCamera);
        } else{
            camera->setLastMousePos(lastX, lastY);
        }
        if (forwardCamera)
        camera->moveForward(cameraScrollDirection);
        forwardCamera = false;


        //std::cout << "-----------------------------------------------------------" << std::endl;
        for (unsigned int i = 0; i < 3; i++)
        {
            // compute direction from the camera to the vertex
            glm::vec4 position = inverseViewProjection * clipSpaceSkyBoxCoordinates[i] * 100.0f;
            glm::vec4 dir = position - glm::vec4(camera->getPosition(), 1.0f);

            //std::cout << "x: " << position.x << " y: " << position.y << " z: " << position.z << std::endl;

            worldSpaceSkyBoxPositions[i] = glm::vec3(position);
            worldSpaceSkyBoxDirections[i] = dir;

        }

        // update positions
        GLRenderer.updateRenderOperationVertexAttribute(skyBoxRenderOperation, 0, 0, worldSpaceSkyBoxPositions.data());
        // update directions
        GLRenderer.updateRenderOperationVertexAttribute(skyBoxRenderOperation, 1, 0, worldSpaceSkyBoxDirections.data());

        skyBoxShader.use();
        skyBoxShader.setMatrix4fv("view", view);
        skyBoxShader.setMatrix4fv("projection", projectionMatrix);
        cameraTexture.update(skyBoxShader);

        
        
        // --------------------------------------------------------------
        // Disable the depth buffer usage so that the background does not write to it
        glDepthMask(GL_FALSE);
        
        // draw background
        GLRenderer.render(skyBoxRenderOperation);
        
        // Enable it after drawing the background
        glDepthMask(GL_TRUE);

        
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    delete camera;
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, bool& rotateCamera)
{

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS)
    {
        rotateCamera = true;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3) == GLFW_RELEASE)
    {
        rotateCamera = false;
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window,double xpos,double ypos)
 {
    if(firstMouse)
    {
        lastX=xpos;
        lastY=ypos;
        firstMouse=false;
    }

    yaw = xpos - lastX;
    pitch = lastY - ypos;

    if (abs(yaw) < 2.5f)
        yaw = 0.0f;
    if (abs(pitch) < 2.5f)
        pitch = 0.0f;
    lastX = xpos;
    lastY = ypos;
    float yawSensitivity = 0.005f;
    float pitchSensitivity = 0.005f;
    yaw   *= yawSensitivity;
    pitch *= pitchSensitivity;

 }

 void mouse_scroll_callback(GLFWwindow*window, double x, double y)
 {
    forwardCamera = true;
    cameraScrollDirection = y;
 }

