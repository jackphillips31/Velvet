#include <vlpch.h>
#include <Velvet.h>

class ExampleLayer : public Velvet::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}
};

class WindowApp : public Velvet::Application
{
public:
	WindowApp()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Velvet::ImGuiLayer());
	}
	~WindowApp()
	{

	}
};

Velvet::Application* Velvet::CreateApplication()
{
	VL_TRACE("Creating Application...");
	return new WindowApp;
}