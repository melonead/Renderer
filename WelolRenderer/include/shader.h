#pragma once
#include "glm/glm.hpp"
#include <string>
#include "WelolMath.h"

class Shader {
public:

	Shader() = default;
	Shader(const std::string& v_path, const std::string& f_path);
	Shader(
		const std::string& v_path, 
		const std::string& tcs_path, 
		const std::string& tes_path, 
		const std::string& f_path
	);

	Shader(const std::string& c_path);

	~Shader();

	void use();
	
	void setBool(const char* name, bool value);

	void setInt(const char* name, int value);

	void setFloat(const char* name, float value);

	void setMatrix4fv(const char* name, glm::mat4& value);

	void wlSetMatrix4fv(const char* name, WelolMath::Mat4x4 &value);

	void setMatrix3fv(const char* name, glm::mat3& value);

	void setVec3f(const char* name, glm::vec3& value);

	void setVec4f(const char* name, glm::vec4& value);

	unsigned int getShaderID() {return shaderID;}

private:
	unsigned int shaderID;
	std::string read_shader_code(const std::string& path);

	void compile_shader(
		const std::string& path, 
		unsigned int shader_type, 
		char infoLog[], 
		const std::string& shader_name
	);

	void link_shaders(char infoLog[]);

	void load_shaders(const std::string& v_path, const std::string& f_path);

	void load_shaders(
		const std::string& v_path, 
		const std::string& tcs_path, 
		const std::string& tes_path, 
		const std::string& f_path
	);

	// This is for compute shaders
	void load_shaders(const std::string& c_path);
};

