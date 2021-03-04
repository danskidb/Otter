#pragma once

#ifdef OT_PLATFORM_WINDOWS

extern Otter::Application* Otter::CreateApplication();

int main(int argc, char* argv[], char* envp[])
{
	Otter::Log::Init();
	OT_CORE_WARN("Initialized Log");

	auto app = Otter::CreateApplication();
	app->Run(argc, argv, envp);
	delete app;
}

#endif