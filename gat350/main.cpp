#include <SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/engine.h"
#include "engine/input/input.h"
#include "engine/renderer/texture.h"
#include "engine/renderer/program.h"
#include "engine/renderer/renderer.h"
#include "engine/renderer/vertex_index_array.h"

const float kWidth = 800.0f;
const float kHeight = 600.0f;

const GLfloat cube_vertices[] = {
	// front
	-1.0, -1.0,  1.0, 1.0, 0.0, 0.0, 0.0, 0.0,
	 1.0, -1.0,  1.0, 0.0, 1.0, 0.0, 1.0, 0.0,
	 1.0,  1.0,  1.0, 0.0, 0.0, 1.0, 1.0, 1.0,
	-1.0,  1.0,  1.0, 1.0, 1.0, 1.0, 0.0, 1.0,
	// back
	-1.0, -1.0, -1.0, 1.0, 0.0, 0.0, 1.0, 1.0,
	 1.0, -1.0, -1.0, 0.0, 1.0, 0.0, 0.0, 1.0,
	 1.0,  1.0, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
	-1.0,  1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 0.0
};

GLushort cube_elements[] = {
	// front
	0, 1, 2,
	2, 3, 0,
	// right
	1, 5, 6,
	6, 2, 1,
	// back
	7, 6, 5,
	5, 4, 7,
	// left
	4, 0, 3,
	3, 7, 4,
	// bottom
	4, 5, 1,
	1, 0, 4,
	// top
	3, 2, 6,
	6, 7, 3
};

std::shared_ptr<Input> input = std::make_shared<Input>();
std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>();

void init() {
	filesystem::set_current_path("assets");

	int result = SDL_Init(SDL_INIT_VIDEO);
	if (result != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
	}

	input->Initialize();
	renderer->Initialize(kWidth, kHeight);
}

glm::vec3 TranslateFromInput() {
	glm::vec3 translate(0.0f, 0.0f, 0.0f);
	float speed = 10.0f;
	if (input->GetKey(SDL_SCANCODE_RIGHT)) translate.x = speed;
	if (input->GetKey(SDL_SCANCODE_LEFT)) translate.x = -speed;
	if (input->GetKey(SDL_SCANCODE_UP))	translate.y = speed;
	if (input->GetKey(SDL_SCANCODE_DOWN)) translate.y = -speed;
	if (input->GetKey(SDL_SCANCODE_W)) translate.z = -speed;
	if (input->GetKey(SDL_SCANCODE_S)) translate.z = speed;

	return translate;
}

int main(int argc, char** argv) {
	init();


#pragma region Shaders
	Program program;
	program.CreateShaderFromFile("shaders/texture_unlit.vert", GL_VERTEX_SHADER);
	program.CreateShaderFromFile("shaders/texture_unlit.frag", GL_FRAGMENT_SHADER);
	program.Link();
	program.Use();
#pragma endregion

#pragma region VIA Initialization
	VertexIndexArray vertex_array;
	vertex_array.CreateBuffer(VertexArray::MULTI, sizeof(cube_vertices), sizeof(cube_vertices) / sizeof(GLfloat), (void*)cube_vertices);
	vertex_array.CreateIndexBuffer(GL_UNSIGNED_SHORT, sizeof(cube_elements) / sizeof(GLushort), (void*)cube_elements);
	vertex_array.SetAttribute(VertexArray::POSITION, 3, 8 * sizeof(GLfloat), 0);
	vertex_array.SetAttribute(VertexArray::COLOR, 3, 8 * sizeof(GLfloat), 3 * sizeof(GLfloat));
	vertex_array.SetAttribute(VertexArray::TEXCOORD, 2, 8 * sizeof(GLfloat), 6 * sizeof(GLfloat));
#pragma endregion


#pragma region Texture Loading
	int width, height, bpp;

	GLuint textures[2];
	glGenTextures(2, textures);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	u8* data = Texture::LoadImage("textures/crate.bmp", width, height, bpp);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	delete data;

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	data = Texture::LoadImage("textures/nc.bmp", width, height, bpp);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	delete data;
#pragma endregion

	glm::mat4 mx_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
	glm::mat4 mx_rotate = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mx_projection = glm::perspective(glm::radians(45.0f), kWidth / kHeight, 0.01f, 1000.0f);

	glm::vec3 eye = glm::vec3(0.0f, 0.0f, 15.0f);
	glm::mat4 mx_view = glm::lookAt(eye, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 mx_model = glm::mat4(1.0f);

	bool quit = false;
	while (!quit) {
		SDL_Event event;
		SDL_PollEvent(&event);

		switch (event.type) {
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				quit = true;
			}
			break;
		}

		SDL_PumpEvents();
		g_timer.tick();

		glm::vec3 translate = TranslateFromInput();

		eye = eye + translate * g_timer.dt;
		mx_view = glm::lookAt(eye, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		//glm::mat4 mx = mx_projection * mx_view * mx_model;

		mx_translate = glm::translate(mx_translate, translate * g_timer.dt);
		mx_rotate = glm::rotate(mx_rotate, glm::radians(45.0f) * g_timer.dt, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 mx_model = mx_translate * mx_rotate;
		glm::mat4 mx = mx_projection * mx_view * mx_model;
		program.SetUniform("mx", mx);

		renderer->ClearBuffer();
		vertex_array.Bind();
		vertex_array.Draw();
		renderer->SwapBuffer();
	}

	input->Shutdown();
	renderer->Shutdown();

	SDL_Quit();

	return 0;
}