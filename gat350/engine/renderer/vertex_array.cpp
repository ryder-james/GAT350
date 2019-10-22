#include "vertex_array.h"

VertexArray::VertexArray() {
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &vao_);
	for (VertexBuffer vertex_buffer : vertex_buffers_) {
		glDeleteBuffers(1, &vertex_buffer.vbo);
	}
}

void VertexArray::CreateBuffer(eAttrib attrib, GLsizei size, GLsizei num_vertex, void* data) {
	vertex_count_ = num_vertex;

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

	VertexBuffer vertex_buffer = { attrib, vbo, num_vertex };
	vertex_buffers_.push_back(vertex_buffer);
}

void VertexArray::SetAttribute(eAttrib attrib, GLint num_vertex, GLsizei stride, size_t offset) {
	glEnableVertexAttribArray(attrib);
	glVertexAttribPointer(attrib, num_vertex, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offset));
}

void VertexArray::Draw(GLenum primitiveType) {
	glBindVertexArray(vao_);
	glDrawArrays(primitiveType, 0, vertex_count_);
	glBindVertexArray(0);
}
