#pragma once

#ifdef VL_PLATFORM_WINDOWS

extern Velvet::Application* Velvet::CreateApplication();

int main(int argc, char** argv)
{
	printf("Welcome to Velvet!");
	auto app = Velvet::CreateApplication();
	app->Run();
	delete app;
}

#endif