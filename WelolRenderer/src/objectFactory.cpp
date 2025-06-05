#include "objectFactory.h"
#include <fstream>
#include "Camera.h"
#include <filesystem>
#include <map>


ObjectFactory::ObjectFactory(std::string& bluePrintPath)
    : bpPath(bluePrintPath)
{

}

void ObjectFactory::createRenderObject(std::string&path)
{
	std::vector<Token> tokens = factoryScanner.scan(path);
	factoryParser = Parser{tokens};
	factoryParser.parse();

}


std::vector<Token> Scanner::scan(std::string& path)
{ 
    std::cout << "scanning file" << std::endl;
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
    
    return Tokens;
} 

void Scanner::addToken(W_TokenType type, std::string& value)
{
	Token t{type, value};
	Tokens.push_back(t);
	std::cout << value << std::endl;
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
	std::string value = source.substr(position + 1, size - 2);
	addToken(STRING, value);
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
	
	std::string value = source.substr(position, size);
	addToken(NUMBER, value);
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
	
	std::string value = source.substr(position, size);

	addToken(IDENTIFIER, value);
}

Parser::Parser(std::vector<Token> tokens)
	:Tokens{std::move(tokens)} 
{
}	

void Parser::parse(Scope& globalScope)
{
	std::cout << "parsing the tokens" << std::endl;
	while (peek().type != END_OF_FILE)
	{
		if (peek().type == EQUAL)
		{
			std::cout << previous().value << " = " << lookOneAhead() << std::endl;
		}

		advance();
	}
}


Scope* Parser::getParentScope(Scope& scope)
{
	//
	return ObjectFactory::globalScope.scopeIsEmpty() &ObjectFactory::globalScope : &ObjectFactory::globalScope.getLastItemPtr(); 
}


Scope::Scope(Scope* parent)
	: par(parent)
{
	//
}

Token Parser::advance()
{
	//if (!isAtEndToken()) current++;
	current++;
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

void ObjectRenderInfo::update(Welol::Renderer& renderer, Welol::Camera& camera, glm::mat4& projectionMatrix, glm::mat4& localMatrix)
{
    shader->use();
    shader->setMatrix4fv("view", camera.getViewMatrix());
    shader->setMatrix4fv("projection", projectionMatrix);
    shader->setMatrix4fv("model", localMatrix);
    if (!(diffuse == nullptr))
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
