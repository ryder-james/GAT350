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
#include "engine/renderer/material.h"
#include "engine/renderer/light.h"
#include "engine/renderer/vertex_index_array.h"

const u32 kWidth = 800;
const u32 kHeight = 600;

static float cube_vertices[] = {
	// Front
	-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	// Right
	 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
	 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
	 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
	 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
	// Back
	-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
	// Left
	-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
	-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
	-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
	-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
	// Bottom
	-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
	-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
	 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
	 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
	// Top
	-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f
};

static GLushort cube_elements[] =
{
	 0,  1,  2,  0,  2,  3,
	 4,  5,  6,  4,  6,  7,
	 8,  9, 10,  8, 10, 11,
	12, 13, 14, 12, 14, 15,
	16, 17, 18, 16, 18, 19,
	20, 21, 22, 20, 22, 23
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

#pragma region VIA Initialization
	VertexIndexArray vertex_array;
	vertex_array.CreateBuffer(VertexArray::MULTI, sizeof(cube_vertices), sizeof(cube_vertices) / sizeof(GLfloat), (void*)cube_vertices);
	vertex_array.CreateIndexBuffer(GL_UNSIGNED_SHORT, sizeof(cube_elements) / sizeof(GLushort), (void*)cube_elements);
	vertex_array.SetAttribute(VertexArray::POSITION, 3, 6 * sizeof(GLfloat), 0);
	vertex_array.SetAttribute(VertexArray::NORMAL, 3, 6 * sizeof(GLfloat), 3 * sizeof(GLfloat));
#pragma endregion

#pragma region Material/Lighting
	Material material;
	material.program = new Program();
	material.program->CreateShaderFromFile("shaders/gouraud.vert", GL_VERTEX_SHADER);
	material.program->CreateShaderFromFile("shaders/gouraud.frag", GL_FRAGMENT_SHADER);
	material.program->Link();
	material.program->Use();

	material.ambient = glm::vec3(1.0f);
	material.diffuse = glm::vec3(0.2f, 0.2f, 1.0f);
	material.specular = glm::vec3(1.0f);
	material.shininess = 32.0f;

	material.Update();
	material.Use();

	Light light;
	light.position = glm::vec4(5.0f, 2.0f, 5.0f, 1.0f);
	light.ambient = glm::vec3(0.1f);
	light.diffuse = glm::vec3(1.0f);
	light.specular = glm::vec3(1.0f);
#pragma endregion

#pragma region Texture Loading
	Texture texture;
	texture.CreateTexture("textures/nc.bmp");
	texture.Bind();
#pragma endregion

#pragma region Cube/Camera Transforms
	glm::mat4 mx_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
	glm::mat4 mx_rotate = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mx_projection = glm::perspective(glm::radians(45.0f), kWidth / (float)kHeight, 0.01f, 1000.0f);

	glm::vec3 eye = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::mat4 mx_view = glm::lookAt(eye, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 mx_model = glm::mat4(1.0f);
#pragma endregion

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

		mx_translate = glm::translate(mx_translate, translate * g_timer.dt);
		mx_rotate = glm::rotate(mx_rotate, glm::radians(45.0f) * g_timer.dt, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 mx_model = mx_translate * mx_rotate;

		glm::mat4 mv_mx = mx_view * mx_model;
		glm::mat4 mvp_mx = mx_projection * mv_mx;

		material.program->SetUniform("mv_matrix", mv_mx);
		material.program->SetUniform("mvp_matrix", mvp_mx);

		light.SetShader(material.program, mx_view);

		renderer->ClearBuffer();
		vertex_array.Draw();
		renderer->SwapBuffer();
	}

	material.Destroy();
	input->Shutdown();
	renderer->Shutdown();

	SDL_Quit();

	return 0;
}