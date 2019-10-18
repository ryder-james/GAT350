#include "vertex_index_array.h"

VertexIndexArray::VertexIndexArray() {}

VertexIndexArray::~VertexIndexArray() {
	glDeleteBuffers(1, &ibo_);
}

void VertexIndexArray::CreateIndexBuffer(GLenum index_type, GLsizei num_index, void* data) {
	index_type_ = index_type;
	index_count_ = num_index;

	glGenBuffers(1, &ibo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
	size_t index_size = (index_type_ == GL_UNSIGNED_SHORT) ? sizeof(GLushort) : sizeof(GLuint);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size * num_index, data, GL_STATIC_DRAW);
}

void VertexIndexArray::Draw(GLenum primitiveType) {
	glBindVertexArray(vao_);
	glDrawElements(primitiveType, index_count_, index_type_, 0);
	glBindVertexArray(0);
}
