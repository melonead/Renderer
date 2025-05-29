#include "objectFactory.h"
#include <fstream>
#include "Camera.h"
#include <filesystem>

ObjectFactory::ObjectFactory(std::string& bluePrintPath)
    : bpPath(bluePrintPath)
{

}

ObjectRenderInfo ObjectFactory::createRenderObject(std::string& objectName, Welol::Renderer& renderer, unsigned int texUnit)
{
    Model_Loader mesh_loader;
    ObjectBluePrint bp = parseBluePrintFile(objectName);
    ObjectRenderInfo result;

    // load mesh information
    mesh_loader.load_model(bp.meshPath);
    std::vector<float> positions = mesh_loader.getPositions();
    std::vector<float> texCoords = mesh_loader.getTexCoords();
    std::vector<unsigned int> indices = mesh_loader.get_indices();
    unsigned int numVertices = mesh_loader.getVerticesCount();
    
    // create render operation
    result.rop = Welol::RenderOperation{Welol::WL_TRIANGLES, numVertices, 0, 0, false, true};
    Welol::VertexAttribute position{0, Welol::WL_FLOAT3, positions.data(), numVertices, false};
    Welol::VertexAttribute texCoord{1, Welol::WL_FLOAT2, texCoords.data(), numVertices, false};
    result.rop.addVertexIndices(indices);
    result.rop.setVerticesToRender(indices.size());
    result.rop.addVertexAttribute(position);
    result.rop.addVertexAttribute(texCoord);
    renderer.initializeRenderOperation(result.rop);


    createShaderFilesTemplate(objectName, bp);
    result.shader = new Shader{bp.shaderPath + '\\' + objectName + "Vertex.glsl", bp.shaderPath + '\\' + objectName + "Fragment.glsl"};


    // create diffuse texture
    std::string texName = "diffuse";
    result.diffuse = new Welol::Texture{Welol::WL_TEX_2D, bp.diffuseTexturePath, 0, texName, texUnit};
    result.diffuse->attachImageData(bp.diffuseTexturePath);

    return result;
}

void ObjectFactory::createShaderFilesTemplate(std::string& objectName, ObjectBluePrint& bp)
{
    std::string vPath = bp.shaderPath + '\\' + objectName + "Vertex.glsl";
    std::string fPath = bp.shaderPath + '\\' + objectName + "Fragment.glsl";

    if (!std::filesystem::exists(vPath))
    {
        std::ofstream vFile(vPath);
        if (vFile.fail())
            std::cerr << "Failed to open file: " << vPath << std::endl;
        vFile << "#version 430 core\n";
        vFile << "layout (location=0) in vec3 position;\n";
        vFile << "layout (location=1) in vec2 texCoord;\n";
        vFile << "uniform mat4 view;\n";
        vFile << "uniform mat4 projection;\n";
        vFile << "uniform mat4 model;\n";
        vFile << "out vec2 fragTexCoord;\n";
        vFile << "void main()\n";
        vFile << "{\n";
        vFile << "  gl_Position = projection * view * model * vec4(position, 1.0);\n";
        vFile << "  fragTexCoord = texCoord;\n";
        vFile << "}\n";
    }

    if (!std::filesystem::exists(fPath))
    {
        std::ofstream fFile(fPath);
        if (fFile.fail())
            std::cerr << "Failed to open file: " << vPath << std::endl;
        
        fFile << "#version 430 core\n";
        fFile << "out vec4 color;\n";
        fFile << "in vec2 fragTexCoord;\n";
        fFile << "uniform sampler2D diffuse;\n";
        fFile << "void main()\n";
        fFile << "{\n";
        fFile << "  color = texture(diffuse, fragTexCoord);\n";
        fFile << "}\n";
    }
}

ObjectBluePrint ObjectFactory::parseBluePrintFile(std::string& objectName)
{
    std::ifstream bpStream{bpPath};
    ObjectBluePrint result;

    std::string line;
    std::string str;
    bool found = false;
    while (!bpStream.eof())
    {
        std::getline(bpStream, line);


        for (auto& ch : line)
        {
            if (ch == ' ')
                continue;
            str += ch;
        }

        if (str == "{")
        {
            str = "";
            continue;
        }
        // the target object has been found and recorded completely
        // move out of the loop.
        // REVISIT: if object is to be composed of other objects, we'll have
        // to think more carefully about this situation.
        if (str == "}" && objectName == result.name)
        {
            break;
        }
        if (str == "}")
        {
            str = "";
            continue;   
        }

        if (str == "")
            continue;


        std::size_t fieldEnd = str.find_first_of(':');

        if (fieldEnd == str.npos)
        {
            std::cerr << "Put ':' at the end of the field name \n";
            std::cout << str << std::endl;
        }
        std::string field = str.substr(0, fieldEnd);

        std::size_t valueEnd = str.find_last_of(',');
        if (valueEnd == str.npos)
        {
            std::cerr << "Put ',' at the end of the field name \n";
            std::cout << str << std::endl;
        }

        std::string value = str.substr(fieldEnd + 1, valueEnd - fieldEnd - 1);


        if (field == "name" && value == objectName)
        {
            found = true;
        }

        if (found)
        {
            if (field == "name")
            {
                result.name = value;
    
            } else if (field == "shaderPath")
            {
                result.shaderPath = value;
            } else if (field == "diffuseTexturePath")
            {
                result.diffuseTexturePath = value;
            } else if (field == "meshPath")
            {
                result.meshPath = value;
            }
        }
        

        str = "";
    }

    return result;
}

void ObjectRenderInfo::update(Welol::Renderer& renderer, Welol::Camera& camera, glm::mat4& projectionMatrix, glm::mat4& localMatrix)
{
    shader->use();
    shader->setMatrix4fv("view", camera.getViewMatrix());
    shader->setMatrix4fv("projection", projectionMatrix);
    shader->setMatrix4fv("model", localMatrix);
    diffuse->update(*shader);
    renderer.render(rop);
}


//--------------------- object render info --------------------------

ObjectRenderInfo::ObjectRenderInfo()
{
    
}

ObjectRenderInfo::~ObjectRenderInfo()
{
    delete shader;
    delete diffuse;
}