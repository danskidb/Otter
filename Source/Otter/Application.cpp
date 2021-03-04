#include "Application.h"
#include "Components/Transform2DComponent.h"
#include "Types/OString.h"

namespace Otter {

	Coordinator* Coordinator::instance = 0;

	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	OString getShit()
	{
		OString shit = "Shit";
		return shit;
	}

	void Application::Run(int argc, char* argv[], char* envp[])
	{
		coordinator = Coordinator::GetInstance();
		RegisterComponents();
		RegisterSystems();

		OString s("yooooo");
		OString a = s;
		const char* test = a.GetCString();
		OString b = getShit();

		OnStart();

		float dt = 0.0f;
		while (true) {
			auto startTime = std::chrono::high_resolution_clock::now();

			OnTick(dt);
			coordinator->OnTick(dt);

			auto stopTime = std::chrono::high_resolution_clock::now();
			dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
		}

		OnStop();
		coordinator->DestroyInstance();
	}

	void Application::RegisterComponents()
	{
		coordinator->RegisterComponent<Transform2DComponent>();
	}

	void Application::RegisterSystems()
	{

	}
}