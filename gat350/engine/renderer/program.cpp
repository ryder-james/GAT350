#include "program.h"
#include "../../core/core.h"

#include "glm/gtc/type_ptr.hpp"

Program::~Program() {
	// return if program was not set
	if (program_ == 0) {
		return;
	}

	// get attached shaders
	GLint num_shaders = 0;
	glGetProgramiv(program_, GL_ATTACHED_SHADERS, &num_shaders);
	std::vector<GLuint> shaders(num_shaders);
	glGetAttachedShaders(program_, num_shaders, NULL, shaders.data());

	// detach and delete shaders
	for (GLuint shader : shaders) {
		glDetachShader(program_, shader);
		glDeleteShader(shader);
	}

	// delete program
	glDeleteProgram(program_);
}

void Program::CreateShaderFromFile(const std::string& filename, GLenum shader_type) {
	// create program if not already created
	if (program_ == 0) {
		program_ = glCreateProgram();
	}

	// get shader source from file
	std::string source;
	bool success = filesystem::read_file(filename.c_str(), source);
	if (success == false) {
		SDL_Log("Error: Failed to open file (%s).", filename.c_str());
	}

	// create shader from source
	CreateShaderFromSource(source, shader_type);
}

void Program::CreateShaderFromSource(const std::string& source, GLenum shader_type) {
	// create shader
	GLuint shader = glCreateShader(shader_type);

	// compile shader
	const char* source_c = source.c_str();
	glShaderSource(shader, 1, &source_c, NULL);
	glCompileShader(shader);

	// check shader compilation status
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		// display shader error
		GLint length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

		if (length > 0) {
			std::string infoLog(length, ' ');
			glGetShaderInfoLog(shader, length, &length, &infoLog[0]);
			SDL_Log("Error: Failed to compile shader.");
			SDL_Log("Shader Info: %s", infoLog.c_str());
		}

		// delete shader
		glDeleteShader(shader);
	} else {
		// success - attach shader
		glAttachShader(program_, shader);
	}
}

void Program::Link() {
	assert(program_ != 0);

	if (linked_) return;

	// link shader programs
	glLinkProgram(program_);

	// check program link status
	GLint status;
	glGetProgramiv(program_, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint length = 0;
		glGetShaderiv(program_, GL_INFO_LOG_LENGTH, &length);

		if (length > 0) {
			std::string infoLog(length, ' ');
			glGetProgramInfoLog(program_, length, &length, &infoLog[0]);

			SDL_Log("Error: Failed to link program.");
			SDL_Log("Program Info: %s", infoLog.c_str());
		}

		glDeleteProgram(program_);

	} else {
		linked_ = true;
	}
}

void Program::Use() {
	ASSERT(program_ && linked_);

	glUseProgram(program_);
}

void Program::SetUniform(const std::string& name, float x, float y, float z) {
	GLint uniform = GetUniform(name);
	glUniform3f(uniform, x, y, z);
}

void Program::SetUniform(const std::string& name, const glm::vec2& v2) {
	GLint uniform = GetUniform(name);
	glUniform2f(uniform, v2.x, v2.y);
}

void Program::SetUniform(const std::string& name, const glm::vec3& v3) {
	GLint uniform = GetUniform(name);
	glUniform3f(uniform, v3.x, v3.y, v3.z);
}

void Program::SetUniform(const std::string& name, const glm::vec4& v4) {
	GLint uniform = GetUniform(name);
	glUniform4f(uniform, v4.x, v4.y, v4.z, v4.w);
}

void Program::SetUniform(const std::string& name, const glm::mat4& mx4) {
	GLint uniform = GetUniform(name);
	glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(mx4));
}

void Program::SetUniform(const std::string& name, const glm::mat3& mx3) {
	GLint uniform = GetUniform(name);
	glUniformMatrix3fv(uniform, 1, GL_FALSE, glm::value_ptr(mx3));
}

void Program::SetUniform(const std::string& name, float value) {
	GLint uniform = GetUniform(name);
	glUniform1f(uniform, value);
}

void Program::SetUniform(const std::string& name, int value) {
	GLint uniform = GetUniform(name);
	glUniform1i(uniform, value);
}

void Program::SetUniform(const std::string& name, bool value) {
	GLint uniform = GetUniform(name);
	glUniform1i(uniform, value);
}

void Program::SetUniform(const std::string& name, GLuint value) {
	GLint uniform = GetUniform(name);
	glUniform1ui(uniform, value);
}

GLint Program::GetUniform(const std::string& name) {
	// find uniform in map
	auto uniform = uniforms_.find(name);
	if (uniform == uniforms_.end()) {
		// add uniform to map
		uniforms_[name] = glGetUniformLocation(program_, name.c_str());
	}

	return uniforms_[name];
}

