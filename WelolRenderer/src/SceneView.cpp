#include "SceneView.h"
#include "Renderer.h"
#include "WelolMath.h"

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

        // Lines.
        std::vector<float> linePositions1 = {
            -axisL, 0.0f, -axisL,
             axisL, 0.0f, -axisL
        };

        std::vector<float> linePositions2 = {
            -axisL, 0.0f, -axisL,
            -axisL, 0.0f,  axisL
        };

        Welol::VertexAttribute linePosition{0, Welol::WL_FLOAT3, linePositions1.data(), 2, false};
        Welol::VertexAttribute linePosition2{ 1, Welol::WL_FLOAT3, linePositions2.data(), 2, false };
        groundLinesRenderOperation.addVertexAttribute(linePosition);
        groundLinesRenderOperation.addVertexAttribute(linePosition2);
        renderer.initializeRenderOperation(groundLinesRenderOperation);

        // 
        // AxisLength is length of the axis from 0 to (-or+)axisLength, ie one arm of the
        // axis.
        // LinesCount is the total lines run both axes.
        lineSpacing = axisLength / ((linesCount / 4) - 1);





        // switch the matrix columns y and z
        /*
        linesModelMatrix[1] = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
        linesModelMatrix[2] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

        axesModelMatrix[1] = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
        axesModelMatrix[2] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
        */
    }

    void SceneView::update(Renderer& renderer, glm::mat4& projection, glm::mat4& view)
    {
        
        linesShader.use();
        linesShader.setMatrix4fv("projectionMatrix", projection);
        linesShader.setMatrix4fv("viewMatrix", view);
        linesShader.setMatrix4fv("model", linesModelMatrix);
        linesShader.setInt("lineSpacing", lineSpacing);
        linesShader.setInt("numberOfLines", linesCount);
        renderer.render(groundLinesRenderOperation);

        
        shader.use();
        shader.setMatrix4fv("projectionMatrix", projection);
        shader.setMatrix4fv("viewMatrix", view);
        shader.setMatrix4fv("model", axesModelMatrix);
        renderer.render(axisRenderOperation);
    }

    void SceneView::wlUpdate(Renderer& renderer, glm::mat4& projection, WelolMath::Mat4x4& view)
    {
        linesShader.use();
        linesShader.setMatrix4fv("projectionMatrix", projection);
        linesShader.wlSetMatrix4fv("viewMatrix", view);
        linesShader.setMatrix4fv("model", linesModelMatrix);
        linesShader.setInt("lineSpacing", lineSpacing);
        linesShader.setInt("numberOfLines", linesCount);
        renderer.render(groundLinesRenderOperation);

        
        shader.use();
        shader.setMatrix4fv("projectionMatrix", projection);
        shader.wlSetMatrix4fv("viewMatrix", view);
        shader.setMatrix4fv("model", axesModelMatrix);
        renderer.render(axisRenderOperation);
    }
}
