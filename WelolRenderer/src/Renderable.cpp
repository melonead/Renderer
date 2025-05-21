#include "Renderable.h"
#include "Renderer.h"

Renderable::Renderable()
{

}

void Renderable::display(Welol::Renderer& renderer)
{

}

void Renderable::addTexture(std::string& path, std::string& name)
{
    textures[name] = Welol::Texture {Welol::WL_TEX_2D, path, 0, name, 0};
    textures[name].attachImageData(path);
}

void Renderable::addShader(std::string& vPath, std::string& fPath)
{
    shader = Shader{vPath, fPath};
}

void Renderable::addVertexAttribute(unsigned int index, Welol::VertexDataType dataType, bool instanced, void* data)
{
    Welol::VertexAttribute attrib{index, dataType, data, renderOp.getVertexCount(), instanced};
    renderOp.addVertexAttribute(attrib);
}

void Renderable::activate(Welol::Renderer& renderer)
{
    renderer.initializeRenderOperation(renderOp);
}
