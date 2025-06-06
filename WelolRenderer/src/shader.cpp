#pragma once
#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include "WelolMath.h"
#include <vector>


void Shader::use() {
    glUseProgram(shaderID);
}

Shader::Shader(const std::string& v_path, const std::string& f_path) 
{
    load_shaders(v_path, f_path);
}

Shader::Shader(
    const std::string& v_path, 
    const std::string& tcs_path, 
    const std::string& tes_path, 
    const std::string& f_path
) {
    load_shaders(v_path, tcs_path, tes_path, f_path);
}

Shader::Shader(const std::string& c_path)
{
    load_shaders(c_path);
}

Shader::~Shader()
{
    glDeleteProgram(shaderID);
    std::cout << "destroyed " << shaderID << std::endl;
}

std::string getFullPath(std::string& fileName)
{
    std::string absolutePath = "C:\\Users\\brian\\programming_projects\\WelolRenderer\\WelolRenderer\\LightingDemo\\shaders";
    absolutePath += '/';
    absolutePath.append(fileName);
    return absolutePath;
}

void mergeSource(std::vector<std::string>& parent, std::vector<std::string>& child, int mergePosition)
{
    for (int i = 0; i < child.size(); i++)
    {
        parent.insert(parent.end(), child[i]);
    }
}

bool checkIfIncludeDirective(std::string& line)
{
    std::string crit = "";
    int critLength = 8;

    auto it = line.begin();
    for (; it != line.end(); it++)
    {
        if (*it == ' ')
        {
            continue;
        }
        if (crit.length() < critLength)
        {
            crit += *it;
        }
    }

    if (crit == "#include")
    {
        return true;
    }
    return false;
}

std::string retreveIncludePath(std::string& includeDirective)
{
    std::size_t start = includeDirective.find_first_of('"');
    std::size_t end = includeDirective.find_last_of('"');

    std::string path = includeDirective.substr(start + 1, end - start - 1);
    return path;
}

std::vector<std::string> readShaderFromFile(const std::string& path)
{
    std::ifstream file_stream{path};

    if (file_stream.fail())
    {
        std::cerr << "Failed to open the file: " << path << std::endl;
    }
    std::vector<std::string> parent;
    std::string line = "";
    int position = 0; 
    std::vector<std::string> child;
    while(!file_stream.eof())
    {
        std::getline(file_stream, line);
        // check if there is an include here.
        if (checkIfIncludeDirective(line))
        {
            std::string fileName = retreveIncludePath(line);
            std::string fullPath = getFullPath(fileName);
            if (fullPath == path)
            {
                std::cerr << "Ignoring attempt to include " << fileName <<   " inside itself" << std::endl;
                std::cout << fullPath << std::endl;
                continue;
            }
            child = readShaderFromFile(fullPath);
            mergeSource(parent, child, position);
        }
        else
        {
            parent.push_back(line + '\n');
            position++;
        }
        // --------------------------------
    }

    file_stream.close();
    return parent;
}



std::string Shader::read_shader_code(const std::string& path) {

    std::vector<std::string> source = readShaderFromFile(path);
    std::string out_put = "";

    for(auto line: source)
    {
        out_put += line;
    }
    
    return out_put;
}

void Shader::compile_shader(
    const std::string& path, 
    unsigned int shader_type, 
    char infoLog[], 
    const std::string& shader_name
) {
    int success;
    std::string Code = read_shader_code(path);
    const char* code_cstr = Code.c_str();

    unsigned int s = glCreateShader(shader_type);
    glShaderSource(s, 1, &code_cstr, NULL);
    glCompileShader(s);
    glGetShaderiv(s, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(s, 512, NULL, infoLog);
        printf("%s shader compilation failed: %s", shader_name.c_str(), infoLog);
    }

    glAttachShader(shaderID, s);
    glDeleteShader(s);
}

void Shader::link_shaders(char infoLog[] ) {
    int success;
    glLinkProgram(shaderID);

    glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderID, 512, NULL, infoLog);
        printf("linking shader program failed: %s ", infoLog);
    }
}

/* load and compile shader source */
void Shader::load_shaders(
    const std::string& v_path, 
    const std::string& f_path
) {
    
    shaderID = glCreateProgram();
    char infoLog[512];

    compile_shader(v_path, GL_VERTEX_SHADER, infoLog, "vertex");
    compile_shader(f_path, GL_FRAGMENT_SHADER, infoLog, "fragment");

    link_shaders(infoLog);
}

void Shader::load_shaders(
    const std::string& v_path, 
    const std::string& tcs_path, 
    const std::string& tes_path, 
    const std::string& f_path
) {
    shaderID = glCreateProgram();
    char infoLog[512];

    compile_shader(v_path, GL_VERTEX_SHADER, infoLog, "vertex");
    compile_shader(tcs_path, GL_TESS_CONTROL_SHADER, infoLog, "tessellation control");
    compile_shader(tes_path, GL_TESS_EVALUATION_SHADER, infoLog, "tessellation evaluation");
    compile_shader(f_path, GL_FRAGMENT_SHADER, infoLog, "fragment");
    
    link_shaders(infoLog);
}

void Shader::load_shaders(const std::string& c_path)
{
    shaderID = glCreateProgram();

    char infoLog[512];

    compile_shader(c_path, GL_COMPUTE_SHADER, infoLog, "compute");

    link_shaders(infoLog);
}


void Shader::setBool(const char* name, bool value)
{
    glUniform1i(glGetUniformLocation(shaderID, name), (int)value);
}

void Shader::setInt(const char* name, int value)
{
    glUniform1i(glGetUniformLocation(shaderID, name), value);
}

void Shader::setFloat(const char* name, float value)
{
    glUniform1f(glGetUniformLocation(shaderID, name), value);
}


void Shader::setMatrix4fv(const char* name, glm::mat4 &value)
{
    glUniformMatrix4fv(glGetUniformLocation(shaderID, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::wlSetMatrix4fv(const char* name, WelolMath::Mat4x4 &value)
{
    glUniformMatrix4fv(glGetUniformLocation(shaderID, name), 1, GL_FALSE, value.getDataPtr());
}

void Shader::setMatrix3fv(const char* name, glm::mat3 &value)
{
    glUniformMatrix3fv(glGetUniformLocation(shaderID, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3f(const char* name, glm::vec3 &value) {
    glUniform3f(glGetUniformLocation(shaderID, name), value.x, value.y, value.z);
}

void Shader::setVec4f(const char* name, glm::vec4& value) {
    glUniform4f(glGetUniformLocation(shaderID, name), value.x, value.y, value.z, value.w);
}