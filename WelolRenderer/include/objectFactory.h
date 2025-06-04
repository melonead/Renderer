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
    Welol::Texture* diffuse = nullptr;
    Shader* shader = nullptr;

    void update(Welol::Renderer& renderer, Welol::Camera& camera, glm::mat4& projectionMatrix, glm::mat4& localMatrix);
};


class Scanner
{
public:
	Scanner() = default;
private:
	friend class ObjectFactory;
    enum TokenType : int
    {
        LEFT_BRACE = 0,
        RIGHT_BRACE = 1,
        MESH_PATH = 2,
        DIFFUSE_TEXTURE_PATH = 3,
        OBJECT_NAME = 4,
        VERTEX_SHADER_PATH = 5,
	EQUAL = 6,
	IDENTIFIER = 7,
	STRING = 8,
	FRAGMENT_SHADER_PATH = 9,
	NUMBER = 10
    };

    std::map<std::string, TokenType> reservedWords
    {
	    {"meshPath", MESH_PATH},
            {"vertexShaderPath", VERTEX_SHADER_PATH},
	    {"fragmentShaderPath", FRAGMENT_SHADER_PATH},
            {"name", OBJECT_NAME},
	    {"diffuseTexturePath", DIFFUSE_TEXTURE_PATH}
    };

    struct Token
    {
        TokenType type;
        std::string value;
    };

	std::string source;  
    std::vector<Token> Tokens;
    std::size_t position{0};
    std::size_t size{0};
    bool scan(std::string& path);
    void addToken(TokenType type, std::string& value);
    char advance();
    bool isAtEndOfFile();
    void addStringToken();
    char lookAhead();
    char lookOneAhead();
    void addNumberToken();
    bool isNumeric(char c);
    void addToken(TokenType type);
};

class Parser
{
public:
    Parser();
    ~Parser();
private:
};


class ObjectFactory
{
public:
    ObjectFactory(std::string& bluePrintPath);
    void createRenderObject(std::string& path);
private:
    std::string bpPath;
    Scanner factoryScanner{};
    void createShaderFilesTemplate(std::string& objectName, ObjectBluePrint& bp);
};
