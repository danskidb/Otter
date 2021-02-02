#include "Application.h"
#include "Components/Transform2DComponent.h"

namespace Otter {

	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		coordinator.Init();
		RegisterComponents();
		RegisterSystems();

		OnStart();

		float dt = 0.0f;
		while (true) {
			auto startTime = std::chrono::high_resolution_clock::now();

			OnTick(dt);

			auto stopTime = std::chrono::high_resolution_clock::now();
			dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
		}

		OnStop();
	}

	void Application::RegisterComponents()
	{
		coordinator.RegisterComponent<Transform2DComponent>();
	}

	void Application::RegisterSystems()
	{

	}
}