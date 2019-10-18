#pragma once
#include <sdl.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <map>
#include <vector>

class Program {
public:
	Program() {}
	~Program();

public:
	void CreateShaderFromFile(const std::string& filename, GLenum shader_type);
	void CreateShaderFromSource(const std::string& source, GLenum shader_type);

	void Link();
	void Use();

	GLuint GetProgramID() { return program_; }
	bool IsLinked() { return linked_; }

	void SetUniform(const std::string& name, float x, float y, float z);
	void SetUniform(const std::string& name, const glm::vec2& v2);
	void SetUniform(const std::string& name, const glm::vec3& v3);
	void SetUniform(const std::string& name, const glm::vec4& v4);
	void SetUniform(const std::string& name, const glm::mat4& mx4);
	void SetUniform(const std::string& name, const glm::mat3& mx3);
	void SetUniform(const std::string& name, float value);
	void SetUniform(const std::string& name, int value);
	void SetUniform(const std::string& name, bool value);
	void SetUniform(const std::string& name, GLuint value);

private:
	GLint GetUniform(const std::string& name);

private:
	GLuint program_ = 0;
	bool linked_ = false;
	std::map<std::string, GLuint> uniforms_;
};
