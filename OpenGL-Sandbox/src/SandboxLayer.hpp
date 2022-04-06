#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

#include "ModelLoader.hpp"


class SandboxLayer : public GLCore::Layer
{
public:
	SandboxLayer();
	virtual ~SandboxLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;
private:
  std::unique_ptr<ModelLoader> model_loader_;
  std::vector<std::unique_ptr<Mesh>> mesh_;

  glm::vec3 velocity = { 0.1f, 0.0f, -0.1f };

  GLCore::Utils::Shader* shader_;
  GLCore::Utils::OrthographicCameraController camera_controller_;
};