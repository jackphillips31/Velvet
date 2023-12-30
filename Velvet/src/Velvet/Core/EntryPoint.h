#pragma once
#include "Velvet/Core/Core.h"

#ifdef VL_PLATFORM_WINDOWS

extern Velvet::Application* Velvet::CreateApplication();

int main(int argc, char** argv)
{
	Velvet::Log::Init(spdlog::level::trace);

	VL_PROFILE_BEGIN_SESSION("Startup", "VelvetProfile-Startup.json");
	Velvet::Application* app = Velvet::CreateApplication();
	VL_PROFILE_END_SESSION();

	VL_PROFILE_BEGIN_SESSION("Runtime", "VelvetProfile-Runtime.json");
	app->Run();
	VL_PROFILE_END_SESSION();

	VL_PROFILE_BEGIN_SESSION("Shutdown", "VelvetProfile-Shutdown.json");
	delete app;
	VL_PROFILE_END_SESSION();
}

#endif