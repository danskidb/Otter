#pragma once

#include "Core/Coordinator.h"

namespace Otter {

	class Application
	{
	public:
		Coordinator coordinator;

		Application();
		virtual ~Application();

		void Run();

		virtual void RegisterComponents();
		virtual void RegisterSystems();

		virtual void OnStart() = 0;
		virtual void OnTick(float deltaTime) = 0;
		virtual void OnStop() = 0;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}