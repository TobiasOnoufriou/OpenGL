#include "SandboxLayer.hpp"
#include "Mesh.hpp"

using namespace GLCore;
using namespace GLCore::Utils;

SandboxLayer::SandboxLayer():
  camera_controller_(16.0f / 9.0f) //States a 16:9 aspect ratio
{
}

SandboxLayer::~SandboxLayer()
{
}

void SandboxLayer::OnAttach()
{
	EnableGLDebugging();
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glLineWidth(1);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  shader_ = Shader::FromGLSLTextFiles(
    "src/vertex.vs", 
    "src/fragment.fs"
  );

  model_loader_ = std::make_unique<ModelLoader>("src/assets/monkey.obj", false);// std::make_unique<ModelLoader>("src/assets/backpack.obj", false);
  mesh_.push_back(std::move(model_loader_->LoadModel("src/assets/monkey.obj")));

  // Mesh push back after creation within sandbox layer.
// In this function all call for model and file name is needed
	// Init here
}

void SandboxLayer::OnDetach()
{
	// Shutdown here
}

void SandboxLayer::OnEvent(Event& event)
{
  camera_controller_.OnEvent(event);
  EventDispatcher dispatcher(event);
  dispatcher.Dispatch<MouseButtonPressedEvent>(
    [&](MouseButtonPressedEvent& e)
    {
      this->mesh_.at(0)->ChangeVertexPosition(velocity, 1);
      return false;
    });


	// Events here
}

void SandboxLayer::OnUpdate(Timestep ts)
{
  camera_controller_.OnUpdate(ts);
	// Render here
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glStencilFunc(GL_ALWAYS, 1, 0xFF);
  glStencilMask(0xFF);

  glUseProgram(shader_->GetRendererID());

  int location = glGetUniformLocation(shader_->GetRendererID(), "u_ViewProjection");
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(this->camera_controller_.GetCamera().GetViewProjectionMatrix()));

  this->mesh_.at(0)->Draw(shader_->GetRendererID());
  //model_loader_->Draw(shader_->GetRendererID());
}

void SandboxLayer::OnImGuiRender()
{
	// ImGui here
}

