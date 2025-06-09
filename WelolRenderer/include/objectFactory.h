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
    std::string vertexShaderPath = "this is an error";
    std::string fragmentShaderPath;
    std::string diffuseTexturePath;
    std::string meshPath;
};

struct ObjectRenderInfo
{

    ObjectRenderInfo() = default;
    ~ObjectRenderInfo();
    Welol::RenderOperation* rop = nullptr;
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
	NUMBER,
	END_OF_FILE
};


struct Token
{
	W_TokenType type;
	std::string lexeme;
    // REVISIT: for now I'm assuming that every literal value will be
    // a string. However, that is obviously going to change in order to 
    // hold other values. Be sure to change this when that time arises.
    // std::string literal;
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
    void addToken(W_TokenType type, std::string& literal);
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
    W_TokenType getIdentifierTokenType(Token& token);
	    
};


class Scope
{
public:
    Scope() = default;
    Scope(Scope* parent);
    
private:
    std::map<std::string, std::string> scopeMap;
    Scope* par;
    std::string name;
    std::string vShaderPath;
    std::string fShaderPath;
    std::string meshPath;
    std::string diffuseTexPath;
    bool scopeIsEmpty();
    Scope* getLastItemPtr();
};


class Parser
{
public:
    Parser() = default;
    Parser(std::vector<Token> tokens);
    void parse(Scope& globalScope, std::map<std::string, ObjectBluePrint>&  bluePrintsMap);
private:
     std::vector<Token> Tokens;
     Scope* globalScopePtr;
     Scope* getParentScope(Scope& scope);
     int current{0};
     int start{0};
     Token advance();
     Token previous();
     Token lookOneAhead();
     Token lookTwoAhead();
     Token peek();
     bool isAtEnd();

};


class ObjectFactory
{
public:
    ObjectFactory(std::string& path);
    ObjectRenderInfo* createRenderObject(std::string& name, Welol::Renderer& renderer);
private:
    friend Parser;
    std::string bpPath;
    Scanner factoryScanner{};
    Parser factoryParser{};
    Scope globalScope{};
    std::map<std::string, ObjectBluePrint> objectBluePrintsMap;
   

    void createShaderFilesTemplate(std::string& objectName, ObjectBluePrint& bp);
};
