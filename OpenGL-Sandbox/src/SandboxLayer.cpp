#include "SandboxLayer.hpp"
#include "Mesh.hpp"
#include "glm/gtx/rotate_vector.hpp"

using namespace GLCore;
using namespace GLCore::Utils;

SandboxLayer::SandboxLayer(uint32_t w, uint32_t h): camera_controller_(16.0f / 9.0f), width(w), height(h)//States a 16:9 aspect ratio
{
}

SandboxLayer::~SandboxLayer()
{
}

float angleBetween(
  glm::vec3 a,
  glm::vec3 b,
  glm::vec3 origin
) {
  glm::vec3 da = glm::normalize(a - origin);
  glm::vec3 db = glm::normalize(b - origin);
  return glm::acos(glm::dot(da, db));
}

void SandboxLayer::OnAttach()
{
	EnableGLDebugging();
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //glLineWidth(1);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  model_loader_ = std::make_unique<ModelLoader>();// std::make_unique<ModelLoader>("src/assets/backpack.obj", false);
  // Scene should also be returned to access the the right node, maybe storing each mesh into a hash map.  
  line_ = std::make_unique<LineTools>();
  mesh_ = std::move(model_loader_->LoadModel("src/assets/monkey.obj"));
  plane_ = std::move(model_loader_->LoadModel("src/assets/Plane.obj"));

  this->kScene = model_loader_->getScene();

  this->mesh_->TranslateShape(glm::vec3(0.0f, 0.0f, 0.0f));
  plane_->TranslateShape(glm::vec3(0.0f, -1.0f, 0.0f));


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
      // BELOW DOESN'T EVEN WORK BECAUSE IT DOESN'T ALLIGN WITH MY TYPE OF INTERSECTION.
      // Add here to send a Ray out.
      //camera_controller_.GetCamera().GetPosition()
      float x = (2.0f * e.GetMouseX()) / width - 1.0f;
      float y = 1.0f - (2.0f * e.GetMouseY()) / height;
      float z = 1.0f;
      glm::vec3 ray_nds = glm::vec3(x, y, z);
      glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0f, 1.0f);


      glm::vec4 ray_eye = glm::inverse(camera_controller_.GetCamera().GetProjectionMatrix()) * ray_clip;
      ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 1.0f);
      glm::vec3 ray_wor = glm::vec3(glm::inverse(camera_controller_.GetCamera().GetViewMatrix()) * -ray_eye);
      glm::mat4 model(1.0f);
      
      //model = glm::rotate(model, glm::radians(angleBetween( camera_controller_.GetCamera().GetUp(), camera_controller_.GetCamera().GetRight(), camera_controller_.GetCamera().GetPosition())), glm::vec3(1.0f, 1.0f, 0.0f));


      glm::vec4 ray_dir = glm::vec4(ray_wor.x, ray_wor.y, ray_wor.z, 0.0f);

      
      // Somewhere below is where ray is fucked. It needs to take in the account the direction of the camera
      ray_wor = glm::inverse(camera_controller_.GetCamera().GetViewMatrix()) * ray_dir;


      line_->InitLine(ray_dir,ray_wor,model);
     
      if (mesh_->intersectionTest(ray_dir, ray_wor)) {
        std::cout << "hit" << std::endl;
      }
      
      //mesh_->TranslateShape();

      //ModelLoader load;
      //load.ProcessNode(kScene->mRootNode, kScene);
      return false;
    });
	// Events here
}

// TODO: Create a DebugShader

void SandboxLayer::OnUpdate(Timestep ts)
{
  camera_controller_.OnUpdate(ts);
	// Render here
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glStencilFunc(GL_ALWAYS, 1, 0xFF);
  glStencilMask(0xFF);
  glLineWidth(2);


  this->mesh_->DrawBoundingBox(this->camera_controller_);
  this->mesh_->Draw(this->camera_controller_);
  this->plane_->Draw(this->camera_controller_);
  this->line_->DrawDebugLine(this->camera_controller_);
}

void SandboxLayer::OnImGuiRender() {
	// ImGui here
}