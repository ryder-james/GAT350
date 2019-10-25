#include <SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/input/input.h"
#include "engine/renderer/texture.h"
#include "engine/renderer/program.h"
#include "engine/renderer/renderer.h"
#include "engine/renderer/material.h"
#include "engine/renderer/light.h"
#include "engine/renderer/vertex_index_array.h"
#include "engine/renderer/mesh.h"
#include "engine/math/math.h"

const u32 kWidth = 800;
const u32 kHeight = 600;

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

void GenerateMesh(VertexArray& vertex_array, std::string mesh_name) {
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;

	std::string s = "meshes/" + mesh_name + ".obj";

	Mesh::Load(s, positions, normals, texcoords);

	//glm::mat3 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//math::transform(positions, rotate);
	//math::transform(normals, rotate);
	if (normals.empty()) {
		for (size_t i = 0; i < positions.size() - 2; i += 3) {
			glm::vec3 normal = math::calculate_normal(positions[i], positions[i + 1], positions[i + 2]);
			normals.push_back(normal);
			normals.push_back(normal);
			normals.push_back(normal);
		}
	}

	if (!positions.empty()) {
		vertex_array.CreateBuffer(VertexArray::POSITION, static_cast<GLsizei>(positions.size() * sizeof(glm::vec3)), static_cast<GLsizei>(positions.size()), (void*)& positions[0]);
		vertex_array.SetAttribute(VertexArray::POSITION, 3, 0, 0);
	}

	if (!normals.empty()) {
		vertex_array.CreateBuffer(VertexArray::NORMAL, static_cast<GLsizei>(normals.size() * sizeof(glm::vec3)), static_cast<GLsizei>(normals.size()), (void*)& normals[0]);
		vertex_array.SetAttribute(VertexArray::NORMAL, 3, 0, 0);
	} 
	if (!texcoords.empty()) {
		vertex_array.CreateBuffer(VertexArray::TEXCOORD, static_cast<GLsizei>(texcoords.size() * sizeof(glm::vec2)), static_cast<GLsizei>(texcoords.size()), (void*)& texcoords[0]);
		vertex_array.SetAttribute(VertexArray::TEXCOORD, 2, 0, 0);
	}
}

int main(int argc, char** argv) {
	init();

	VertexArray vertex_array;
	GenerateMesh(vertex_array, "ogre");

#pragma region Material/Lighting
	Material material;
	material.program = new Program();
	material.program->CreateShaderFromFile("shaders/texture_phong.vert", GL_VERTEX_SHADER);
	material.program->CreateShaderFromFile("shaders/texture_phong.frag", GL_FRAGMENT_SHADER);
	material.program->Link();
	material.program->Use();

	material.ambient = glm::vec3(0.2f);
	material.diffuse = glm::vec3(1.0f);
	material.specular = glm::vec3(0.2f);
	material.shininess = 8.0f;

	Texture* texture = new Texture();
	texture->CreateTexture("textures/ogre/diffuse.bmp");
	material.textures.push_back(texture);

	material.Update();
	material.Use();

	Light light;
	light.position = glm::vec4(5.0f, 2.0f, 5.0f, 1.0f);
	light.ambient = glm::vec3(0.1f);
	light.diffuse = glm::vec3(1.0f);
	light.specular = glm::vec3(1.0f);
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

		//eye = eye + translate * g_timer.dt;
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