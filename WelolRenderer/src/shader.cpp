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

std::string Shader::read_shader_code(const std::string& path) {
    std::ifstream file_stream{path, std::ios_base::in};
    std::string out_put;
    std::stringstream source_stream;

    source_stream << file_stream.rdbuf();
    out_put = source_stream.str();
    
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