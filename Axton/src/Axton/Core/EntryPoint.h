#pragma once

#ifdef AX_PLAT_WINDOWS

extern Axton::Application* Axton::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Axton::CreateApplication();
	app->Run();
	delete app;
}

#endif