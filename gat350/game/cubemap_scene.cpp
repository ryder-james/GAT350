#include "cubemap_scene.h"

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

bool CubemapScene::Create(const Name& name) {
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
		shader->CreateShaderFromFile("shaders/skybox.vert", GL_VERTEX_SHADER);
		shader->CreateShaderFromFile("shaders/skybox.frag", GL_FRAGMENT_SHADER);
		shader->Link();
		engine_->Resources()->Add("skybox_shader", std::move(shader));
	}

	{
		auto shader = engine_->Factory()->Create<Program>(Program::GetClassName());
		shader->name_ = "shader";
		shader->engine_ = engine_;
		shader->CreateShaderFromFile("shaders/reflection.vert", GL_VERTEX_SHADER);
		shader->CreateShaderFromFile("shaders/reflection.frag", GL_FRAGMENT_SHADER);
		shader->Link();
		engine_->Resources()->Add("reflection_shader", std::move(shader));
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

	// material
	{
		auto material = engine_->Factory()->Create<Material>(Material::GetClassName());
		material->name_ = "material";
		material->engine_ = engine_;
		material->ambient = glm::vec3(1.0f);
		material->diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
		material->specular = glm::vec3(1.0f);
		material->shininess = 128.0f;

		auto texture = std::make_unique<Texture>();
		std::vector<std::string> suffixes = { "posx", "negx", "posy", "negy", "posz", "negz" };

		std::vector<std::string> names = Texture::GenerateCubeMapNames("textures/", suffixes, ".jpg");
		texture->CreateCubeTexture(names);
		engine_->Resources()->Add("cube_texture", std::move(texture));

		material->textures.push_back(engine_->Resources()->Get<Texture>("cube_texture"));
		engine_->Resources()->Add("material", std::move(material));
	}

	// scene actors

	// model
	{
		auto model = engine_->Factory()->Create<Model>(Model::GetClassName());
		model->name_ = "model1";
		model->engine_ = engine_;
		model->scene_ = this;
		model->transform_.translation = glm::vec3(0.0f);
		model->transform_.scale = glm::vec3(1);
		model->mesh_ = engine_->Resources()->Get<Mesh>("meshes/cube.obj");
		model->mesh_->flags_ = BIT(Mesh::CULL_FRONT);// | BIT(Mesh::ENABLE_DEPTH_TEST);
		model->mesh_->material_ = engine_->Resources()->Get<Material>("cube_material");
		model->shader_ = engine_->Resources()->Get<Program>("skybox_shader");
		Add(std::move(model));
	}

	{
		auto model = engine_->Factory()->Create<Model>(Model::GetClassName());
		model->name_ = "model2";
		model->engine_ = engine_;
		model->scene_ = this;
		model->transform_.translation = glm::vec3(0.0f);
		model->transform_.scale = glm::vec3(1);
		model->mesh_ = engine_->Resources()->Get<Mesh>("meshes/suzanne.obj");
		model->mesh_->material_ = engine_->Resources()->Get<Material>("cube_material");
		model->shader_ = engine_->Resources()->Get<Program>("reflection_shader");
		Add(std::move(model));
	}

	// light
	{
		auto light = engine_->Factory()->Create<Light>(Light::GetClassName());
		light->name_ = "light";
		light->engine_ = engine_;
		light->scene_ = this;
		light->Create("light");
		light->transform_.translation = glm::vec3(0.2f, 2, 0.2f);
		light->transform_.rotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(1, 0, 0));
		light->ambient = glm::vec3(0.3f);
		light->diffuse = glm::vec3(1);
		light->specular = glm::vec3(0);
		light->cutoff = 30.0f;
		Add(std::move(light));
	}

	// camera
	{
		auto camera = engine_->Factory()->Create<Camera>(Camera::GetClassName());
		camera->name_ = "camera";
		camera->engine_ = engine_;
		camera->scene_ = this;
		camera->Create("camera");
		camera->transform_.translation = glm::vec3(0.0f, 0.0f, 5.0f);
		camera->transform_.rotation = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		camera->SetProjection(45.0f, 1280.0f / 720.0f, 0.01f, 100.0f);

		Add(std::move(camera));
	}

	return true;
}

void CubemapScene::Update() {
	Scene::Update();

	Light* light = Get<Light>("light");
	light->transform_.translation = light->transform_.translation * glm::angleAxis(glm::radians(45.0f) * g_timer.dt, glm::vec3(0, 1, 0));
	light->SetShader(engine_->Resources()->Get<Program>("phong_shader").get());


	Model* model = Get<Model>("model2");
	model->transform_.rotation = model->transform_.rotation * glm::angleAxis(glm::radians(45.0f) * g_timer.dt, glm::vec3(0, 1, 0));

	GUI::Update(engine_->GetEvent());
	GUI::Begin(engine_->Get<Renderer>());

	engine_->Get<Editor>()->UpdateGUI();

	GUI::End();
}

void CubemapScene::Draw() {
	engine_->Get<Renderer>()->ClearBuffer();

	Scene::Draw();
	GUI::Draw();

	engine_->Get<Renderer>()->SwapBuffer();
}