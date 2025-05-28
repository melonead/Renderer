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
    // ----------------------------- Renderer -----------------------------------------
    Welol::Renderer GLRenderer;
    // --------------------------------------------------------------------------------

    // --------------------------------- instanced draw call demo ----------------------------------

    stbi_set_flip_vertically_on_load(true);
    // 
    // GPU PROGRAM

    std::string vertexShaderPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\shaders\\rectVertex.glsl";
    std::string fragmentShaderPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\shaders\\rectFragment.glsl";
    
    Shader triangleShader{ vertexShaderPath, fragmentShaderPath };

    std::string instanceRectsVertexShaderPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\shaders\\instancedRectsVertex.glsl";
    std::string instanceRectsFragmentShaderPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\shaders\\instancedRectsFragment.glsl";
    Shader instancedRectsShader{ instanceRectsVertexShaderPath , instanceRectsFragmentShaderPath };

    // Camera Shader.
    std::string cameraVertexShaderPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\shaders\\cameraVertex.glsl";
    std::string cameraFragmentShaderPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\shaders\\cameraFragment.glsl";
    Shader cameraShader{ cameraVertexShaderPath , cameraFragmentShaderPath };

    // Camera model path.
    std::string cameraObjModelPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\3DModels\\bridge.obj";

    // Model Loader.
    Model_Loader cameraObjLoader;
    cameraObjLoader.load_model(cameraObjModelPath);

    std::vector<float> cameraVertexPositions = cameraObjLoader.getPositions();

    std::vector<unsigned int> cameraVertexIndices = cameraObjLoader.get_indices();
    std::vector<float> cameraTextureCoordinates = cameraObjLoader.getTexCoords();
    unsigned int cameraVerticesCount = cameraVertexIndices.size();

    Welol::RenderOperation cameraRenderOperation{ Welol::WL_TRIANGLES, cameraVerticesCount, 0, 0, false, true };

    // add vertex indices
    cameraRenderOperation.addVertexIndices(cameraVertexIndices);
    
    // attach vertex position attributes
    Welol::VertexAttribute cameraPosition{ 0, Welol::WL_FLOAT3, cameraVertexPositions.data(), cameraVerticesCount, false};
    // attach vertex texture coordinate attributes
    Welol::VertexAttribute cameraTexCoord{ 1, Welol::WL_FLOAT2, cameraTextureCoordinates.data(), cameraVerticesCount, false};

    // add the attributes to the render operation
    cameraRenderOperation.addVertexAttribute(cameraPosition);
    cameraRenderOperation.addVertexAttribute(cameraTexCoord);

    // ----------- initialization ----------------------
    // initialize render operation
    GLRenderer.initializeRenderOperation(cameraRenderOperation);
    // TextureTarget target, TextureInternalFormat internalFormat, std::string& imagePath, unsigned int mipLevel, std::string& tName
    // std::string cameraTexturePath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\FluidSim\\particleGradient.png";
    std::string cameraTexturePath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\FluidSim\\particleGradient.png";
    std::string cameraTextureName = "cameraTexture";
    unsigned int texUnit = 0;
    Welol::Texture cameraTexture{Welol::WL_TEX_2D, cameraTexturePath, 0, cameraTextureName, texUnit};
    cameraTexture.attachImageData(cameraTexturePath);
    // --------------------------------------------------------------------------------------------


    // -------------------------- scene view

    float axisLength = 80.0f;
    Welol::SceneView sceneView{axisLength, GLRenderer};
    // -------------------------------------

    // render loop
    // -----------

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

        glFrontFace(GL_CCW);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);

        // ---------------------------------------------- instanced rects

        glm::mat4 view = camera->getViewMatrix();
        sceneView.update(GLRenderer, projectionMatrix, view);

        // WelolMath::Mat4x4 wlView = camera->wLGetViewMatrix();
        // sceneView.wlUpdate(GLRenderer, projectionMatrix, wlView);

        
        cameraShader.use();
        cameraShader.setMatrix4fv("view", view);
        cameraShader.setMatrix4fv("projection", projectionMatrix);
        cameraTexture.update(cameraShader);
        GLRenderer.render(cameraRenderOperation);
        

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
        
        
        // --------------------------------------------------------------
            


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

