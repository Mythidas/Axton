#include <Axton.h>

class SandboxApplication : public Axton::Application
{
public:
	SandboxApplication()
	{

	}

	~SandboxApplication()
	{

	}
};

Axton::Application* Axton::CreateApplication()
{
	return new SandboxApplication();
}