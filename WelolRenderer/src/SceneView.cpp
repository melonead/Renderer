#include "SceneView.h"
#include "Renderer.h"

namespace Welol{
    SceneView::SceneView(float axisL, Welol::Renderer renderer)
        : axisLength(axisL)
    {
        std::vector<float> axisStartsAndEnds  {
            -axisL, 0.0f, 0.0f,
             axisL, 0.0f, 0.0f,

             0.0f, -axisL, 0.0f,
             0.0f,  axisL, 0.0f,

             0.0f, 0.0f, -axisL,
             0.0f, 0.0f,  axisL
        };

        std::vector<float> axisColors {
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f
        };

        Welol::VertexAttribute position{ 0, Welol::WL_FLOAT3, axisStartsAndEnds.data(), numberOfVertices, false };
        Welol::VertexAttribute color{ 1, Welol::WL_FLOAT3, axisColors.data(), numberOfVertices, false };
        axisRenderOperation.addVertexAttribute(position);
        axisRenderOperation.addVertexAttribute(color);
        renderer.initializeRenderOperation(axisRenderOperation);
    }

    void SceneView::update(Renderer& renderer, glm::mat4& projection, glm::mat4& view)
    {
        shader.use();
        glLineWidth(5);
        shader.setMatrix4fv("projectionMatrix", projection);
        shader.setMatrix4fv("viewMatrix", view);
        renderer.render(axisRenderOperation);
    }
}