#include "shadowmap_scene.h"

#include "../engine/engine.h"

#include "../engine/editor/editor.h"

#include "../engine/renderer/renderer.h"
#include "../engine/renderer/program.h"
#include "../engine/renderer/texture.h"
#include "../engine/renderer/material.h"
#include "../engine/renderer/light.h"
#include "../engine/renderer/mesh.h"
#include "../engine/renderer/model.h"
#include "../engine/renderer/camera.h"
#include "../engine/renderer/gui.h"
#include "../engine/renderer/framebuffer.h"

constexpr auto SHADOWMAP_SIZE = 512;

bool ShadowMapScene::Create(const Name& name) {
	// shader
	{
		auto shader = engine_->Factory()->Create<Program>(Program::GetClassName());
		shader->name_ = "shader";
		shader->engine_ = engine_;
		shader->CreateShaderFromFile("shaders/texture_phong.vert", GL_VERTEX_SHADER);
		shader->CreateShaderFromFile("shaders/texture_phong_light.frag", GL_FRAGMENT_SHADER);
		shader->Link();
		engine_->Resources()->Add("phong_shader", std::move(shader));
	}

	{
		auto shader = engine_->Factory()->Create<Program>(Program::GetClassName());
		shader->name_ = "shader";
		shader->engine_ = engine_;
		shader->CreateShaderFromFile("shaders/depth_debug.vert", GL_VERTEX_SHADER);
		shader->CreateShaderFromFile("shaders/depth_debug.frag", GL_FRAGMENT_SHADER);
		shader->Link();
		engine_->Resources()->Add("depth_debug_shader", std::move(shader));
	}

	{
		auto shader = engine_->Factory()->Create<Program>(Program::GetClassName());
		shader->name_ = "shader";
		shader->engine_ = engine_;
		shader->CreateShaderFromFile("shaders/basic_color.vert", GL_VERTEX_SHADER);
		shader->CreateShaderFromFile("shaders/basic.frag", GL_FRAGMENT_SHADER);
		shader->Link();
		engine_->Resources()->Add("debug_shader", std::move(shader));
	}

	// framebuffer
	{
		auto framebuffer = engine_->Factory()->Create<Framebuffer>(Framebuffer::GetClassName());
		framebuffer->Create("framebuffer");
		framebuffer->CreateDepthbuffer(SHADOWMAP_SIZE, SHADOWMAP_SIZE);

		auto texture = engine_->Factory()->Create<Texture>(Texture::GetClassName());
		texture->CreateTexture(SHADOWMAP_SIZE, SHADOWMAP_SIZE, GL_TEXTURE_2D, GL_DEPTH_COMPONENT);
		engine_->Resources()->Add("render_texture", std::move(texture));

		framebuffer->AttachTexture(engine_->Resources()->Get<Texture>("render_texture"), GL_DEPTH_ATTACHMENT);
		framebuffer->Unbind();
		engine_->Resources()->Add("framebuffer", std::move(framebuffer));
	}

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

	// render material
	material = engine_->Factory()->Create<Material>(Material::GetClassName());
	material->name_ = "material";
	material->engine_ = engine_;
	material->ambient = glm::vec3(1.0f);
	material->diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	material->specular = glm::vec3(1.0f);
	material->shininess = 128.0f;
	texture = engine_->Resources()->Get<Texture>("render_texture");
	material->textures.push_back(texture);
	engine_->Resources()->Add("render_material", std::move(material));

	// debug material
	material = engine_->Factory()->Create<Material>(Material::GetClassName());
	material->name_ = "material";
	material->engine_ = engine_;
	material->ambient = glm::vec3(1.0f);
	material->diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	material->specular = glm::vec3(1.0f);
	material->shininess = 128.0f;
	engine_->Resources()->Add("debug_material", std::move(material));

	// scene actors

	// model
	auto model = engine_->Factory()->Create<Model>(Model::GetClassName());
	model->name_ = "model1";
	model->engine_ = engine_;
	model->scene_ = this;
	model->transform_.translation = glm::vec3(0.0f);
	model->transform_.scale = glm::vec3(1);
	model->mesh_ = engine_->Resources()->Get<Mesh>("meshes/suzanne.obj");
	model->mesh_->material_ = engine_->Resources()->Get<Material>("material");
	model->shader_ = engine_->Resources()->Get<Program>("phong_shader");
	Add(std::move(model));

	model = engine_->Factory()->Create<Model>(Model::GetClassName());
	model->name_ = "model2";
	model->engine_ = engine_;
	model->scene_ = this;
	model->transform_.translation = glm::vec3(0, -4, 0);
	model->transform_.scale = glm::vec3(20);
	model->mesh_ = engine_->Resources()->Get<Mesh>("meshes/plane.obj");
	model->mesh_->material_ = engine_->Resources()->Get<Material>("material");
	model->shader_ = engine_->Resources()->Get<Program>("phong_shader");
	Add(std::move(model));

	model = engine_->Factory()->Create<Model>(Model::GetClassName());
	model->name_ = "model3";
	model->engine_ = engine_;
	model->scene_ = this;
	model->transform_.translation = glm::vec3(0, 0, 0);
	model->transform_.scale = glm::vec3(1);
	model->mesh_ = engine_->Resources()->Get<Mesh>("meshes/quad.obj");
	model->mesh_->material_ = engine_->Resources()->Get<Material>("render_material");
	model->shader_ = engine_->Resources()->Get<Program>("depth_debug_shader");
	Add(std::move(model));

	// light
	auto light = engine_->Factory()->Create<Light>(Light::GetClassName());
	light->name_ = "light";
	light->engine_ = engine_;
	light->scene_ = this;
	light->Create("light");
	light->transform_.translation = glm::vec3(1, -0.5f, 1);
	light->transform_.rotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(1, 0, 0));
	light->ambient = glm::vec3(0.5f);
	light->diffuse = glm::vec3(1);
	light->specular = glm::vec3(1);
	light->cutoff = 30.0f;
	light->exponent = 8.0f;
	Add(std::move(light));

	// camera
	{
		auto camera = engine_->Factory()->Create<Camera>(Camera::GetClassName());
		camera->name_ = "camera";
		camera->engine_ = engine_;
		camera->scene_ = this;
		camera->user_camera_ = true;
		camera->Create("camera");
		camera->transform_.translation = glm::vec3(0.0f, 0.0f, 5.0f);
		camera->transform_.rotation = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		camera->SetProjection(45.0f, 1280.0f / 720.0f, 0.01f, 100.0f);
		Add(std::move(camera));
	}

	{
		auto camera = engine_->Factory()->Create<Camera>(Camera::GetClassName());
		camera->name_ = "buffer_camera";
		camera->engine_ = engine_;
		camera->scene_ = this;
		//camera->Create("buffer_camera");
		camera->transform_.translation = glm::vec3(0.0f, 0.0f, 5.0f);
		camera->transform_.rotation = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		camera->SetProjection(45.0f, 1, 0.01f, 100.0f);
		Add(std::move(camera));
	}


	return true;
}

void ShadowMapScene::Update() {
	Scene::Update();

	{
		auto model = Get<Model>("model1");
		model->transform_.rotation = model->transform_.rotation * glm::angleAxis(glm::radians(15.0f) * g_timer.dt, glm::vec3(0, -1, 0));
	}

	Light* light = Get<Light>("light");
	light->transform_.translation = light->transform_.translation * glm::angleAxis(glm::radians(45.0f) * g_timer.dt, glm::vec3(0, -1, 0));
	light->SetShader(engine_->Resources()->Get<Program>("phong_shader").get());

	GUI::Update(engine_->GetEvent());
	GUI::Begin(engine_->Get<Renderer>());

	engine_->Get<Editor>()->UpdateGUI();

	GUI::End();
}

void ShadowMapScene::Draw() {
	RenderToTexture();
	RenderToScene();

	//Scene::Draw();
	GUI::Draw();
	engine_->Get<Renderer>()->SwapBuffer();
}

void ShadowMapScene::RenderToTexture() {
	auto framebuffer = engine_->Resources()->Get<Framebuffer>("framebuffer");
	framebuffer->Bind();

	SetActive<Camera>("buffer_camera");
	engine_->Get<Renderer>()->SetViewport(0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE);
	engine_->Get<Renderer>()->ClearBuffer();

	Scene::Draw();
	//auto model = Get<Model>("model1");
	//model->Draw();

	framebuffer->Unbind();
}

void ShadowMapScene::RenderToScene() {
	auto renderer = engine_->Get<Renderer>();
	SetActive<Camera>("camera");
	//auto camera = Get<Camera>("camera");
	//camera->SetProjection(45, static_cast<float>(renderer->GetWidth()) / renderer->GetHeight(), 0.01f, 100);

	renderer->RestoreViewport();
	renderer->ClearBuffer();

	auto model = Get<Model>("model3");
	model->Draw();
	//Scene::Draw();
}