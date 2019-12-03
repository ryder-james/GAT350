#include "framebuffer.h"

Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &framebuffer_);
}

bool Framebuffer::Create(const Name& name) {
	name_ = name;

	glGenFramebuffers(1, &framebuffer_);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

	return true;
}

void Framebuffer::AttachTexture(const std::shared_ptr<Texture>& texture, GLenum attachment) {
	texture_ = texture;

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture_->texture_, 0);

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	ASSERT(result == GL_FRAMEBUFFER_COMPLETE);
}

void Framebuffer::CreateDepthbuffer(u32 width, u32 height) {

}

void Framebuffer::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
}

void Framebuffer::Unbind() {
	glFlush();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}