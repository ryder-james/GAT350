#pragma once
#include "../engine.h"

class VertexArray {
public:
	enum eAttrib {
		POSITION = 0,
		NORMAL,
		COLOR,
		TEXCOORD,
		MULTI,
		TANGENT
	};

	struct VertexBuffer {
		eAttrib attrib;
		GLuint vbo;
		GLsizei num;
	};

	VertexArray();
	~VertexArray();

	void CreateBuffer(eAttrib attrib, GLsizei size, GLsizei num_vertex, void* data);
	void SetAttribute(eAttrib attrib, GLint num_vertex, GLsizei stride, size_t offset);

	virtual void Draw(GLenum primitiveType = GL_TRIANGLES);

	void Bind() { glBindVertexArray(vao_); }

protected:
	GLuint vao_ = 0;
	GLuint vertex_count_ = 0;
	std::vector<VertexBuffer> vertex_buffers_;
};

