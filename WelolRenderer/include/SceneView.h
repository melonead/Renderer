#pragma once
#include "Renderer.h"

namespace Welol {


    static std::string sceneViewVertexShaderPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\shaders\\sceneViewVertex.glsl";
    static std::string sceneViewFragmentShaderPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\shaders\\sceneViewFragment.glsl";
    
    static std::string sceneViewLinesVertexShaderPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\shaders\\sceneViewLinesVertex.glsl";
    static std::string sceneViewLinesFragmentShaderPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\shaders\\sceneViewLinesFragment.glsl";
    
    class SceneView
    {
    public:
        SceneView(float axisL, Welol::Renderer renderer);
        void update(Renderer& renderer, glm::mat4& projection, glm::mat4& view);
        void wlUpdate(Renderer& renderer, glm::mat4& projection, WelolMath::Mat4x4& view);
    private:
        glm::mat4 linesModelMatrix = glm::mat4(1.0f);
        glm::mat4 axesModelMatrix = glm::mat4(1.0f);

        WelolMath::Mat4x4 wlLinesModelMatrix{1.0f};
        WelolMath::Mat4x4 wlAxesModelMatrix{1.0f};


        float axisLength{0.0f};
        unsigned int numberOfVertices{6};
        Welol::RenderOperation axisRenderOperation{ Welol::WL_LINES, numberOfVertices, 0, 0, false, false };
        Shader shader{Welol::sceneViewVertexShaderPath, Welol::sceneViewFragmentShaderPath};

        unsigned int linesCount = 82;
        unsigned int lineSpacing {0};
        Welol::RenderOperation groundLinesRenderOperation{ Welol::WL_LINES, 2, 0, linesCount, true, false };
        Shader linesShader{Welol::sceneViewLinesVertexShaderPath, Welol::sceneViewLinesFragmentShaderPath};
    };

}