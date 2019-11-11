#include "game_scene.h"
#include "../engine/engine.h"
#include "../engine/renderer/renderer.h"
#include "../engine/renderer/program.h"
#include "../engine/renderer/texture.h"
#include "../engine/renderer/material.h"
#include "../engine/renderer/light.h"
#include "../engine/renderer/mesh.h"
#include "../engine/renderer/model.h"
#include "../engine/renderer/camera.h"
#include "../engine/renderer/gui.h"

bool GameScene::Create(const Name& name) {
	// shader
	auto shader = engine_->Factory()->Create<Program>(Program::GetClassName());
	shader->name_ = "shader";
	shader->engine_ = engine_;
	shader->CreateShaderFromFile("shaders/texture_phong.vert", GL_VERTEX_SHADER);
	shader->CreateShaderFromFile("shaders/texture_phong.frag", GL_FRAGMENT_SHADER);
	shader->Link();
	engine_->Resources()->Add("phong_shader", std::move(shader));

	// material
	auto material = engine_->Factory()->Create<Material>(Material::GetClassName());
	material->name_ = "material";
	material->engine_ = engine_;
	material->ambient = glm::vec3(1.0f);
	material->diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	material->specular = glm::vec3(1.0f);
	material->shininess = 128.0f;

	// texture
	auto texture = engine_->Resources()->Get<Texture>("textures/uvgrid.jpg");
	material->textures.push_back(texture);
	engine_->Resources()->Add("material", std::move(material));

	// scene actors

	// model
	auto model = engine_->Factory()->Create<Model>(Model::GetClassName());
	model->name_ = "model";
	model->engine_ = engine_;
	model->scene_ = this;
	model->transform_.translation = glm::vec3(0.0f);
	model->transform_.scale = glm::vec3(0.5f);
	model->mesh_ = engine_->Resources()->Get<Mesh>("meshes/sphere.obj");
	model->mesh_->m_material = engine_->Resources()->Get<Material>("material");
	model->shader_ = engine_->Resources()->Get<Program>("phong_shader");
	Add(std::move(model));

	// light
	auto light = engine_->Factory()->Create<Light>(Light::GetClassName());
	light->name_ = "light";
	light->engine_ = engine_;
	light->scene_ = this;
	light->transform_.translation = glm::vec3(10.0f);
	light->ambient = glm::vec3(0.1f);
	light->diffuse = glm::vec3(1.0f);
	light->specular = glm::vec3(1.0f);
	Add(std::move(light));

	// camera
	auto camera = engine_->Factory()->Create<Camera>(Camera::GetClassName());
	camera->name_ = "camera";
	camera->engine_ = engine_;
	camera->scene_ = this;
	camera->transform_.translation = glm::vec3(0.0f, 0.0f, 5.0f);
	camera->transform_.rotation = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	camera->SetProjection(45.0f, 1280.0f / 720.0f, 0.01f, 100.0f);

	Add(std::move(camera));

	return true;
}

void GameScene::Update() {
	Scene::Update();

	Model* model = Get<Model>("model");
	glm::quat r = glm::angleAxis(glm::radians(45.0f) * g_timer.dt, glm::vec3(0, 1, 0));
	model->transform_.rotation = model->transform_.rotation * r;

	// set shader uniforms
	Light* light = Get<Light>("light");
	//light->SetShader(engine_->Resources()->Get<Program>("phong_shader").get());

	// gui
	GUI::Update(engine_->GetEvent());
	GUI::Begin(engine_->Get<Renderer>());
	GUI::End();
}

void GameScene::Draw() {
	engine_->Get<Renderer>()->ClearBuffer();

	GUI::Draw();
	Scene::Draw();

	engine_->Get<Renderer>()->SwapBuffer();
}
