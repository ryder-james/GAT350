#pragma once
#include "vertex_array.h"

class VertexIndexArray : public VertexArray {
public:
	VertexIndexArray();
	virtual ~VertexIndexArray();

	void CreateIndexBuffer(GLenum index_type, GLsizei num_index, void* data);

	void Draw(GLenum primitiveType = GL_TRIANGLES) override;

protected:
	GLuint ibo_ = 0;
	GLuint index_count_ = 0;
	GLenum index_type_ = 0;
};