#pragma once
#include "Renderer.h"

namespace Welol {


    static std::string sceneViewVertexShaderPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\sceneViewVertex.glsl";
    static std::string sceneViewFragmentShaderPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\resource\\sceneViewFragment.glsl";
    
    class SceneView
    {
    public:
        SceneView(float axisL, Welol::Renderer renderer);
        void update(Renderer& renderer, glm::mat4& projection, glm::mat4& view);
    private:
        float axisLength{0.0f};
        unsigned int numberOfVertices{6};
        Welol::RenderOperation axisRenderOperation{ Welol::WL_LINES, numberOfVertices, 0, 0, false, false };
        Shader shader{Welol::sceneViewVertexShaderPath, Welol::sceneViewFragmentShaderPath};
    };

}