#pragma once
#include "../engine/framework/scene.h"

class ShadowMapScene : public Scene
{
public:
	OBJECT_DECLARATION(ShadowMapScene, Scene);

	bool Create(const Name& name) override;
	void Update() override;
	void Draw() override;

protected:
	void RenderToTexture();
	void RenderToScene();
};