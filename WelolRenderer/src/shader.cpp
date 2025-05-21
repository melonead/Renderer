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

void processIncludeDirective(std::string& shaderName, std::vector<std::string>& writeTo)
{
    std::string absolutePath = "C:/Users/brian/programming_projects/WelolRenderer/WelolRenderer/src/Demos/Lighting/shaders";
    absolutePath += '/';
    absolutePath.append(shaderName);
    //absolutePath += shaderName;

    std::ifstream file_stream{absolutePath, std::ios_base::in};
    std::string out_put = "";
    std::stringstream source_stream;

    source_stream << file_stream.rdbuf();
    out_put = source_stream.str();

    writeTo[writeTo.size() - 1] = out_put;
    writeTo[writeTo.size() - 1] += '\n';

    // paste the stread at #include shaderName location
    // save the current position on write file 
    // save the string from the read file
    // write string to the write file at the saved position

    file_stream.close();
}

// URGENT: make better error message incase a file is not found

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
}

std::string Shader::read_shader_code(const std::string& path) {
    std::ifstream file_stream{path};

    if (file_stream.fail())
    {
        std::cerr << "Failed to open the file: " << path << std::endl;
    }

    std::vector<std::string> source;
    std::string out_put = "";
    std::stringstream source_stream;

    std::string line = "";
    std::string crit = "";
    int critLength = 8;
    bool printLine = false;
    while(!file_stream.eof())
    {
        std::getline(file_stream, line);
        // Find the include directive
        source.push_back(line + '\n');
        std::size_t first = line.find_first_of('#');
        if (first == line.npos)
        {
            continue;
        }
        
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
        // If #include has been found proceed to paste contents of the 
        // included file.
        if (crit == "#include")
        {
            std::size_t start = line.find_first_of('"');
            std::size_t end = line.find_last_of('"');

            if (start == line.npos || end == line.npos)
            {
                std::cerr << "Incorrect include syntax" << std::endl;
            }

            std::string path = line.substr(start + 1, end - start - 1);
            
            // replace the #include directive at this location with some strings
            // std::string l = "some random stuff";
            // source[source.size() - 1] = l;
            // source[source.size() - 1] += '\n';
            std::cout << "path: " << path << std::endl;
            processIncludeDirective(path, source);
            int d = 3;
        }

        crit = "";
        line = "";
    }
    
    // source_stream << file_stream.rdbuf();
    // out_put = source_stream.str();

    for(auto line: source)
    {
        out_put += line;
    }

    std::cout << out_put << std::endl;
    


    file_stream.close();
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