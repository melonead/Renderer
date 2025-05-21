#pragma once
#include "Renderer.h"
#include "Texture.h"

/*
    REVISIT:
    This is not very efficient for these reasons:
        1. Each renderable object will have its own resources, so there will be duplication of resources.

*/

class Renderable
{
public:
    void display(Welol::Renderer& renderer);
    void addTexture(std::string& path, std::string& name);
    void addShader(std::string& vPath, std::string& fPath);
    void addVertexAttribute(unsigned int index, Welol::VertexDataType dataType, bool instanced, void* data);
    void activate(Welol::Renderer& renderer);
private:
    Welol::RenderOperation renderOp;
    Shader shader;
    std::map<std::string, Welol::Texture> textures;
};