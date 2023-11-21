#pragma once

#ifdef VL_PLATFORM_WINDOWS

extern Velvet::Application* Velvet::CreateApplication();

int main(int argc, char** argv)
{
	Velvet::Log::Init(spdlog::level::trace);

	auto app = Velvet::CreateApplication();
	app->Run();
	delete app;
}

#endif