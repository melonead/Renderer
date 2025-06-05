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


enum W_TokenType : int
{
    LEFT_BRACE,
	RIGHT_BRACE,
	MESH_PATH,
	DIFFUSE_TEXTURE_PATH,
	OBJECT_NAME,
	VERTEX_SHADER_PATH,
	EQUAL,
	IDENTIFIER,
	STRING,
	FRAGMENT_SHADER_PATH,
	NUMBER
};


struct Token
{
	W_TokenType type;
	std::string value;
};

class Scanner
{
public:
	Scanner() = default;
private:
	friend class ObjectFactory;
	friend class Parser;

    

    std::map<std::string, W_TokenType> reservedWords
    {
	    {"meshPath", MESH_PATH},
            {"vertexShaderPath", VERTEX_SHADER_PATH},
	    {"fragmentShaderPath", FRAGMENT_SHADER_PATH},
            {"name", OBJECT_NAME},
	    {"diffuseTexturePath", DIFFUSE_TEXTURE_PATH}
    };


    std::string source;  
    std::vector<Token> Tokens;
    
    std::size_t position{0};
    std::size_t size{0};

    std::vector<Token> scan(std::string& path);
    void addToken(W_TokenType type, std::string& value);
    char advance();
    bool isAtEndOfFile();
    void addStringToken();
    char lookAhead();
    char lookOneAhead();
    void addNumberToken();
    bool isNumeric(char c);
    void addToken(W_TokenType type);
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);
    void addIdentifierToken();
};


class Scope
{
public:
    Scope(Scope* parent);
    ~Scope();
private:
    std::map<std::string, std::string> scopeMap;
    Scope* par;
    std::string name;
    std::string vShaderPath;
    std::string fShaderPath;
    std::string meshPath;
    std::string diffuseTexPath;
    bool scopeIsEmpty() {return scopeMap.size() > 0 false : true;}
    Scope* getLastItemPtr()
    {
	    return &scopeMap[scopeMap.size()];
    }
};


class Parser
{
public:
    Parser() = default;
    Parser(std::vector<Token> tokens);
    void parse(Scope& globalScope);
private:
     std::vector<Token> Tokens;
     Scope* globalScopePtr;
     Scope* getParentScope(Scope& scope);
     int current{0};
     int start{0};
     Token advance();
     Token previous();
     Token lookOneAhead();
     Token peek();

};


class ObjectFactory
{
public:
    ObjectFactory(std::string& bluePrintPath);
    void createRenderObject(std::string& path);
private:
    friend Parser;
    std::string bpPath;
    Scanner factoryScanner{};
    Parser factoryParser{};
    static Scope globalScope;

    void createShaderFilesTemplate(std::string& objectName, ObjectBluePrint& bp);
};
