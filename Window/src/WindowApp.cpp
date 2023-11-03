#include <vlpch.h>
#include <Velvet.h>

class ExampleLayer : public Velvet::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnEvent(Velvet::Event& event) override
	{
		VL_TRACE(event);
	}
};

class WindowApp : public Velvet::Application
{
public:
	WindowApp()
	{
		PushLayer(new ExampleLayer());
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