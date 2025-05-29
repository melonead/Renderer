#pragma once
#include "Renderer.h"
#include "objloader.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"

// REVIST: replace name of object with enum
struct ObjectBluePrint
{
    std::string name;
    std::string shaderPath;
    std::string diffuseTexturePath;
    std::string meshPath;
};

struct ObjectRenderInfo
{

    ObjectRenderInfo();
    ~ObjectRenderInfo();
    Welol::RenderOperation rop;
    Welol::Texture* diffuse;
    Shader* shader;

    void update(Welol::Renderer& renderer, Welol::Camera& camera, glm::mat4& projectionMatrix, glm::mat4& localMatrix);
};


class ObjectFactory
{
public:
    ObjectFactory(std::string& bluePrintPath);
    ObjectRenderInfo createRenderObject(std::string& objectName, Welol::Renderer& renderer, unsigned int texUnit);
private:
    std::string bpPath;
    ObjectBluePrint parseBluePrintFile(std::string& objectName);
};