#pragma once

#include <Otter.h>

class SandboxApp : public Otter::Application
{
public:
	SandboxApp();
	~SandboxApp();

	void RegisterComponents() override;
	void OnStart() override;
	void OnTick(float deltaTime) override;
	void OnStop() override;
};

Otter::Application* Otter::CreateApplication()
{
	return new SandboxApp();
}
