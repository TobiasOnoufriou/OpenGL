#include "GLCore.h"
#include "SandboxLayer.hpp"

using namespace GLCore;

class Sandbox : public Application
{
public:
	Sandbox()
	{
    
		PushLayer(new SandboxLayer(GetWindow().GetWidth(), GetWindow().GetHeight()));
	}
};

int main()
{
	std::unique_ptr<Sandbox> app = std::make_unique<Sandbox>();
  app->Run();
}