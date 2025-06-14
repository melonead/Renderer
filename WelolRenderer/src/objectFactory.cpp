#include "objectFactory.h"
#include <fstream>
#include "Camera.h"
#include <filesystem>
#include <map>


ObjectFactory::ObjectFactory(std::string& path)
    : bpPath(path)
{
    std::vector<Token> tokens = factoryScanner.scan(path);
    factoryParser = Parser{tokens};
    factoryParser.parse(globalScope, objectBluePrintsMap);
}

ObjectRenderInfo* ObjectFactory::createRenderObject(std::string& name, Welol::Renderer& renderer)
{
    ObjectRenderInfo* ori = new ObjectRenderInfo{};

    Model_Loader modelLoader;

    modelLoader.load_model(objectBluePrintsMap[name].meshPath);

    std::vector<float> vertexPositions = modelLoader.getPositions();
    std::vector<float> texCoords = modelLoader.getTexCoords();
    std::vector<unsigned int> indices = modelLoader.get_indices();
    unsigned int vertexCount = indices.size();
    //	
    ori->rop = new Welol::RenderOperation{Welol::WL_TRIANGLES, vertexCount, 0, 0, false, true};
    ori->rop->addVertexIndices(indices);

    unsigned int pcount = vertexPositions.size() / 3;
    Welol::VertexAttribute position{0, Welol::WL_FLOAT3, vertexPositions.data(), pcount, false};

    unsigned int tccount = texCoords.size() / 2;
    Welol::VertexAttribute texCoord{1, Welol::WL_FLOAT2, texCoords.data(), tccount, false};
    //
    ori->rop->addVertexAttribute(position);
    ori->rop->addVertexAttribute(texCoord);

    renderer.initializeRenderOperation(*ori->rop);
    //
    std::string texName = "diffuse";
    ori->diffuse = new Welol::Texture{Welol::WL_TEX_2D, 
        objectBluePrintsMap[name].diffuseTexturePath,
        0,
        texName,
        0
    };


    ori->diffuse->attachImageData(objectBluePrintsMap[name].diffuseTexturePath);

    createShaderFilesTemplate(name, objectBluePrintsMap[name]);

    ori->shader = new Shader{
        objectBluePrintsMap[name].vertexShaderPath,
            objectBluePrintsMap[name].fragmentShaderPath
    };


    ori->min = modelLoader.getMin();
    ori->max = modelLoader.getMax();


    return ori;
}


std::vector<Token> Scanner::scan(std::string& path)
{ 
    std::ifstream file_stream{path};
    if (file_stream.fail())
    {
        std::cout << "failed to the open file" << std::endl;
        throw;
    }

    std::stringstream sourceStream;
    sourceStream << file_stream.rdbuf();
    source = sourceStream.str();

    while (!isAtEndOfFile())
    {

        char c = advance();
        switch(c)
        {
            case '{':
                {
                    addToken(LEFT_BRACE);
                    break;
                }
            case '}':
                {	
                    addToken(RIGHT_BRACE);
                    break;
                }
            case '=':
                {
                    addToken(EQUAL);
                    break;
                }
            case '"':
                {
                    addStringToken();
                    break;
                }
            case ' ':
            case '\r':
            case '\t': break;
            case '\n': break;
            default:
               {
                   if (isNumeric(c))
                   {
                       addNumberToken();
                   }
                   else if (isAlpha(c))
                   {
                       addIdentifierToken();
                   }
                   else
                   {
                       std::cerr << "Unexpected character has been encountered: "  << c << std::endl;
                   }
                   break;
               }

        }
        position += size;
        size = 0;
    }
    addToken(END_OF_FILE);

    return Tokens;
} 

void Scanner::addToken(W_TokenType type, std::string& literal)
{
    Token t{type, literal};
    if (type == IDENTIFIER)
        t.type = getIdentifierTokenType(t);

    Tokens.push_back(t);
}

void Scanner::addToken(W_TokenType type)
{
    std::string empty = "";
    addToken(type, empty);
}

bool Scanner::isAtEndOfFile()
{
    return (position == source.size());
}

char Scanner::advance()
{
    size++;
    return source.at(position);
}

void Scanner::addStringToken()
{
    while(lookAhead() != '"' && !isAtEndOfFile())
    {
        advance();
    }

    if (isAtEndOfFile())
    {
        std::cerr << "Unterminated string" << std::endl;
    }

    // advance for the closing "
    advance();
    std::string literal = source.substr(position + 1, size - 2);
    addToken(STRING, literal);
}

void Scanner::addNumberToken()
{
    // Get the whole number part of the number
    while(isNumeric(lookAhead()) && !isAtEndOfFile())
    {
        advance();
    }

    // Get the fractional part
    if (lookAhead() == '.' && isNumeric(lookOneAhead()))
    {

        advance(); // Get past '.'
        while (isNumeric(lookAhead()) && !isAtEndOfFile())
        {
            advance();
        }	
    }

    std::string literal = source.substr(position, size);
    addToken(NUMBER, literal);
}

char Scanner::lookAhead()
{
    if (isAtEndOfFile()) return '\0';
    return source.at(position + size);
}

char Scanner::lookOneAhead()
{
    return source.at(position + size + 1);
}

bool Scanner::isNumeric(char c)
{
    return c >= '0' && c <= '9';
}

bool Scanner::isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

bool Scanner::isAlphaNumeric(char c)
{
    return isAlpha(c) || isNumeric(c);
}


void Scanner::addIdentifierToken()
{
    while(isAlphaNumeric(lookAhead()) && !isAtEndOfFile())
    {
        advance();
    }

    std::string literal = source.substr(position, size);

    addToken(IDENTIFIER, literal);
}

W_TokenType Scanner::getIdentifierTokenType(Token& token)
{
    if (reservedWords.contains(token.lexeme))
        return reservedWords[token.lexeme];

    return token.type;
}


Parser::Parser(std::vector<Token> tokens)
    :Tokens{std::move(tokens)} 
{
}	

void Parser::parse(Scope& globalScope, std::map<std::string, ObjectBluePrint>& bluePrintsMap)
{
    // predefine it here to appease the compiler and override it latter
    ObjectBluePrint bp;
    while (peek().type != END_OF_FILE)
    {

        if (peek().type == RIGHT_BRACE)
        {
            ObjectBluePrint objBp = bp;
            bluePrintsMap[bp.name] = objBp;
        }

        if (peek().type == EQUAL)
        {
            switch (previous().type)
            {
                case OBJECT_NAME:
                    {
                        bp.name = lookOneAhead().lexeme;
                        break;

                    }
                case VERTEX_SHADER_PATH:
                    {
                        bp.vertexShaderPath = lookOneAhead().lexeme;
                        break;
                    }
                case FRAGMENT_SHADER_PATH:
                    {
                        bp.fragmentShaderPath = lookOneAhead().lexeme;
                        break;
                    }
                case DIFFUSE_TEXTURE_PATH:
                    {
                        bp.diffuseTexturePath = lookOneAhead().lexeme;
                        break;
                    }
                case MESH_PATH:
                    {
                        bp.meshPath = lookOneAhead().lexeme;
                        break;
                    }

                default: break;

            }
        }

        advance();
    }
}


Scope::Scope(Scope* parent)
    : par(parent)
{
    //
}

Token Parser::advance()
{
    if (!isAtEnd()) current++;
    return Tokens.at(current);
}

Token Parser::previous()
{
    return Tokens.at(current - 1);
}

Token Parser::peek()
{
    return Tokens.at(current);
}

Token Parser::lookOneAhead()
{
    return Tokens.at(current + 1);
}

bool Parser::isAtEnd()
{
    return (peek().type == END_OF_FILE);
}

Token Parser::lookTwoAhead()
{
    return Tokens.at(current + 2);
}

void ObjectFactory::createShaderFilesTemplate(std::string& objectName, ObjectBluePrint& bp)
{

    if (!std::filesystem::exists(bp.vertexShaderPath))
    {
        std::ofstream vFile(bp.vertexShaderPath);
        if (vFile.fail())
            std::cerr << "Failed to open file: " << bp.vertexShaderPath << std::endl;

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

    if (!std::filesystem::exists(bp.fragmentShaderPath))
    {
        std::ofstream fFile(bp.fragmentShaderPath);
        if (fFile.fail())
            std::cerr << "Failed to open file: " << bp.fragmentShaderPath << std::endl;

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

void ObjectRenderInfo::update(Welol::Renderer& renderer, Welol::Camera& camera, glm::mat4& projectionMatrix, glm::mat4& localMatrix)
{
    shader->use();
    shader->setMatrix4fv("view", camera.getViewMatrix());
    shader->setMatrix4fv("projection", projectionMatrix);
    shader->setMatrix4fv("model", localMatrix);
    if (!(diffuse == nullptr))
        diffuse->update(*shader);

    renderer.render(*rop);
}


//--------------------- object render info --------------------------


ObjectRenderInfo::~ObjectRenderInfo()
{
    delete shader;
    delete diffuse;
    delete rop;
}
