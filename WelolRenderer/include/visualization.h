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

        Welol::VertexAttribute quadPositions{0, Welol::WL_FLOAT2, quadVertices.data(), 6, false};
        Welol::VertexAttribute circleCenters{1, Welol::WL_FLOAT3, circleVertices.data(), 2, true};
        Circle.addVertexAttribute(quadPositions);
        Circle.addVertexAttribute(circleCenters);
        renderer.initializeRenderOperation(Circle);
        
        Welol::VertexAttribute quadLoopPositions{0, Welol::WL_FLOAT2, quadLoopVertices.data(), 4, false};
        Rectangle.addVertexAttribute(quadLoopPositions);
        renderer.initializeRenderOperation(Rectangle);

        Welol::VertexAttribute pos{0, Welol::WL_FLOAT3, lineVertices.data(), 2, false};
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

    void drawCircle(glm::vec3& center, float radius)
    {
        float quadWidth = 0.05f;
        float scale = radius / quadWidth;

        circleModel = glm::translate(glm::mat4(1.0f), center);
        circleModel = glm::scale(circleModel, glm::vec3(scale));
        circleShader.use();
        circleShader.setMatrix4fv("model", circleModel);
        circleShader.setFloat("radius", radius);

        circleVertices.push_back(center);
    }

    void drawLine(glm::vec3& start, glm::vec3& end)
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
        unsigned int posAttIndex = 0;
        unsigned int bufferOffset = 0;
        renderer.updateRenderOperationVertexAttribute(Line, posAttIndex, bufferOffset, lineVertices.data());
        renderer.render(Line);
        // REVISIT: how can we avoid allocation of memory and simply
        // use the one that was previously allocated since we are not
        // changing the amount of memory needed.
        lineVertices.reserve(maxLines);

        // update circles
        circleShader.use();
        circleShader.setMatrix4fv("view", viewMatrix);
        circleShader.setMatrix4fv("projection", projectionMatrix);
        posAttIndex = 1;
        bufferOffset = 0;
        renderer.updateRenderOperationVertexAttribute(Circle, posAttIndex, bufferOffset, circleVertices.data());

        renderer.render(Circle);
        glEnable(GL_CULL_FACE);
        circleVertices.reserve(maxCircles);
    }

private:
    unsigned int maxCircles{30};
    unsigned int maxLines{50};
    Welol::RenderOperation Circle{Welol::WL_TRIANGLES, 6, 0, maxCircles, true, false};
    Welol::RenderOperation Rectangle{Welol::WL_LINE_LOOP, 4, 0, maxCircles, false, false};
    Welol::RenderOperation Line{Welol::WL_LINES, 2 , 0, maxLines, true, false};
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
    std::vector<glm::vec3> circleVertices; // centers of the circles

    glm::mat4 rectModel{1.0f};
    glm::mat4 circleModel{1.0f};
    glm::mat4 projectionMatrix{1.0f};
    glm::mat4 viewMatrix{1.0f};
    glm::mat4 lineModel{1.0f};
    Welol::Renderer renderer{};

};
