#pragma once
#include "Renderer.h"


class Visualization
{
public:

    Visualization() = default;
    Visualization(
        Welol::Renderer& rend,
        glm::mat4& projectionMat
    )
        : projectionMatrix{projectionMat},
          renderer{rend}
    {

        std::vector<float> quadVertices = {
            -0.05f, 0.05f, 
             0.05f,-0.05f, 
            -0.05f,-0.05f, 

            -0.05f, 0.05f,
             0.05f,-0.05f, 
             0.05f, 0.05f
        };

        std::vector<float> quadLoopVertices = {
            -0.05f, 0.05f, 
            0.05f, 0.05f,
            0.05f,-0.05f, 
            -0.05f,-0.05f
        };


        lineVertices.reserve(maxLines);
        circleVertices.reserve(maxCircles);
        circleRadii.reserve(maxCircles);
        circleModelMatrices.reserve(maxCircles);

        Welol::VertexAttribute quadPositions{0, Welol::WL_FLOAT2, quadVertices.data(), 6, false};
        Welol::VertexAttribute circleCenters{1, Welol::WL_FLOAT3, circleVertices.data(), maxCircles, true};
        Welol::VertexAttribute circleRadius{2, Welol::WL_FLOAT, circleRadii.data(), maxCircles, true};
        Welol::VertexAttribute circleModelMatrix{3, Welol::WL_MAT4, circleModelMatrices.data(), maxCircles, true};

        Circle.addVertexAttribute(quadPositions);
        Circle.addVertexAttribute(circleCenters);
        Circle.addVertexAttribute(circleRadius);
        Circle.addVertexAttribute(circleModelMatrix);

        renderer.initializeRenderOperation(Circle);
        
        Welol::VertexAttribute quadLoopPositions{0, Welol::WL_FLOAT2, quadLoopVertices.data(), 4, false};
        Rectangle.addVertexAttribute(quadLoopPositions);
        renderer.initializeRenderOperation(Rectangle);

        Welol::VertexAttribute pos{0, Welol::WL_FLOAT3, lineVertices.data(), maxLines, false};
        Line.addVertexAttribute(pos);
        renderer.initializeRenderOperation(Line);
        


    }

    ~Visualization()
    {
        std::cout << "visualization is now destroyed" << std::endl;
    }

    void drawRectangle(float x, float y, float width, float height)
    {
        float lineThickness = 0.175f;

        float quadWidth = 0.05f;
        float scaleX = (width * 0.5f) / quadWidth;
        float scaleY = (height * 0.5f) / quadWidth;
        
        rectModel = glm::translate(glm::mat4(1.0f), glm::vec3(x + (width * 0.5f), y - (height * 0.5f), 0.0f));
        rectModel = glm::scale(rectModel, glm::vec3(scaleX, scaleY, 1.0f));

        rectangleShader.use();
        rectangleShader.setMatrix4fv("view", viewMatrix);
        rectangleShader.setMatrix4fv("projection", projectionMatrix);
        rectangleShader.setMatrix4fv("model", rectModel);

        rectangleShader.setInt("cx", x);
        rectangleShader.setInt("cy", y);

        rectangleShader.setInt("width", width);
        rectangleShader.setInt("height", height);
  
        renderer.render(Rectangle);
    }

    void drawCircle(const glm::vec3& center, float radius)
    {
        float quadWidth = 0.05f;
        float scale = radius / quadWidth;

        glm::mat4 circleModel = glm::translate(glm::mat4(1.0f), center);
        circleModel = glm::scale(circleModel, glm::vec3(scale));

        circleModelMatrices.push_back(circleModel);
        circleVertices.push_back(center);
        circleRadii.push_back(radius);
    }

    void drawLine(const glm::vec3& start, const glm::vec3& end)
    {

        lineVertices.push_back(start);
        lineVertices.push_back(end);
    }

    void setPerspectiveMatrix(glm::mat4& matrix)
    {
        projectionMatrix = matrix;
    }

    void setViewMatrix(glm::mat4& matrix)
    {
        viewMatrix = matrix;
    }

    void setRenderer(Welol::Renderer& rend)
    {
        renderer = rend;
    }

    void update()
    {

        glDisable(GL_CULL_FACE);
        // update lines
        lineShader.use();
        lineShader.setMatrix4fv("view", viewMatrix);
        lineShader.setMatrix4fv("projection", projectionMatrix);
        renderer.updateRenderOperationVertexAttribute(Line, 0, 0, lineVertices.data());
        renderer.render(Line);
        // REVISIT: how can we avoid allocation of memory and simply
        // use the one that was previously allocated since we are not
        // changing the amount of memory needed.
        lineVertices.clear();

        // update circles
        circleShader.use();

        circleShader.setMatrix4fv("view", viewMatrix);
        circleShader.setMatrix4fv("projection", projectionMatrix);

        renderer.updateRenderOperationVertexAttribute(Circle, 1, 0, circleVertices.data());
        renderer.updateRenderOperationVertexAttribute(Circle, 2, 0, circleRadii.data());
        renderer.updateRenderOperationVertexAttribute(Circle, 3, 0, circleModelMatrices.data());

        renderer.render(Circle);
        circleVertices.clear();
        circleRadii.clear();
        circleModelMatrices.clear();

        glEnable(GL_CULL_FACE);
    }

public:
    unsigned int maxCircles{50};
    unsigned int maxLines{50};
    Welol::RenderOperation Circle{Welol::WL_TRIANGLES, 6, 0, maxCircles, true, false};
    Welol::RenderOperation Rectangle{Welol::WL_LINE_LOOP, 4, 0, maxCircles, false, false};
    Welol::RenderOperation Line{Welol::WL_LINES, maxLines , 0, 0, false, false};
    // Shader circleShader;
    std::string circleVertexShaderPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\shaders\\circleVertex.glsl";
    std::string circleFragmentShaderPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\shaders\\circleFragment.glsl";


    // rectangle shader
    std::string rectVertexShaderPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\shaders\\rectangleVertex.glsl";
    std::string rectFragmentShaderPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\shaders\\rectangleFragment.glsl";
    

    // line shader
    std::string lineVertexShaderPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\shaders\\lineVertex.glsl";
    std::string lineFragmentShaderPath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\shaders\\lineFragment.glsl";

    Shader circleShader{circleVertexShaderPath, circleFragmentShaderPath};
    Shader rectangleShader{rectVertexShaderPath, rectFragmentShaderPath};
    Shader lineShader{lineVertexShaderPath, lineFragmentShaderPath};

    std::vector<glm::vec3> lineVertices;
    std::vector<glm::vec3> circleVertices; // centers of the circle
    std::vector<float> circleRadii;
    std::vector<glm::mat4> circleModelMatrices;

    glm::mat4 rectModel{1.0f};
    glm::mat4 projectionMatrix{1.0f};
    glm::mat4 viewMatrix{1.0f};
    glm::mat4 lineModel{1.0f};
    Welol::Renderer renderer{};

};
