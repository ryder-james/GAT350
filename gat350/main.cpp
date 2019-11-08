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
#include "engine/renderer/gui.h"

const u32 kWidth = 1280;
const u32 kHeight = 720;

int steps = 4;

std::shared_ptr<Input> input = std::make_shared<Input>();
std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>();

void init() {
	Name::AllocNames();

	filesystem::set_current_path("assets");

	int result = SDL_Init(SDL_INIT_VIDEO);
	if (result != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
	}

	input->Initialize();
	renderer->Initialize(kWidth, kHeight);

	GUI::Initialize(renderer.get());
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
	GenerateMesh(vertex_array, "suzanne");
	Transform transform;

	Light light("light");
	Material material;
	std::unique_ptr<Program> shader = std::make_unique<Program>();

	shader->CreateShaderFromFile("shaders/gouraud.vert", GL_VERTEX_SHADER);
	shader->CreateShaderFromFile("shaders/gouraud.frag", GL_FRAGMENT_SHADER);
	shader->Link();
	shader->Use();

	material.ambient = glm::vec3(0.2f);
	material.diffuse = glm::vec3(1.0f);
	material.specular = glm::vec3(0.2f);
	material.shininess = 8.0f;

	std::unique_ptr<Texture> texture = std::make_unique<Texture>();
	texture->CreateTexture("textures/crate.bmp");
	material.textures.push_back(std::move(texture));

	std::unique_ptr<Texture> spec_map = std::make_unique<Texture>();
	spec_map->CreateTexture("textures/crate_specular.bmp", GL_TEXTURE_2D, GL_TEXTURE1);
	material.textures.push_back(std::move(spec_map));

	material.SetShader(shader.get());
	material.Use();

	light.GetTransform().translation = glm::vec3(10.0f, 2.0f, 10.0f);
	light.ambient = glm::vec3(0.1f);
	light.diffuse = glm::vec3(1.0f);
	light.specular = glm::vec3(1.0f);

	glm::mat4 mx_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
	glm::mat4 mx_rotate = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mx_projection = glm::perspective(glm::radians(45.0f), kWidth / (float)kHeight, 0.01f, 1000.0f);

	glm::vec3 eye = glm::vec3(0.0f, 0.0f, 5.0f);
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

		transform.translation += translate * g_timer.dt;
		transform.rotation = transform.rotation * glm::angleAxis(glm::radians(45.0f) * g_timer.dt, glm::vec3(0.0f, 1.0f, 0.0f));

		//eye = eye + translate * g_timer.dt;
		mx_view = glm::lookAt(eye, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		//mx_translate = glm::translate(mx_translate, translate * g_timer.dt);
		//mx_rotate = glm::rotate(mx_rotate, glm::radians(45.0f) * g_timer.dt, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 mx_model = mx_translate * mx_rotate;

		glm::mat4 mv_mx = mx_view * transform.GetMatrix();
		glm::mat4 mvp_mx = mx_projection * mv_mx;

		shader->SetUniform("mv_matrix", mv_mx);
		shader->SetUniform("mvp_matrix", mvp_mx);
		//shader->SetUniform("steps", steps);
		shader->SetUniform("fog.min_distance", 10.0f);
		shader->SetUniform("fog.max_distance", 30.0f);
		shader->SetUniform("fog.color", glm::vec3(0.85f));

		light.SetShader(shader.get(), mx_view);
		material.SetShader(shader.get());

		GUI::Update(event);
		GUI::Begin(renderer.get());

		ImGui::Text("Hello World!");
		light.Edit();
		material.Edit();
		//ImGui::Button("Pause");
		//ImGui::SliderInt("Steps", &steps, 1, 16);

		GUI::End();

		renderer->ClearBuffer();
		GUI::Draw();
		vertex_array.Draw();
		renderer->SwapBuffer();
	}

	GUI::Shutdown();

	material.Destroy();
	input->Shutdown();
	renderer->Shutdown();

	SDL_Quit();

	Name::FreeNames();

	return 0;
}