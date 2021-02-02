#pragma once

#include <Otter.h>

namespace RpgGame {
	class SandboxApp : public Otter::Application
	{
	public:
		void RegisterComponents() override;
		void RegisterSystems() override;

		void OnStart() override;
		void OnTick(float deltaTime) override;
		void OnStop() override;
	};
}

Otter::Application* Otter::CreateApplication()
{
	return new RpgGame::SandboxApp();
}
