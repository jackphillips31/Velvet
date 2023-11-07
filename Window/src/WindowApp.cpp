#include <vlpch.h>
#include <Velvet.h>

class ExampleLayer : public Velvet::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		VL_TRACE("ExampleLayer::OnUpdate");
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
		PushOverlay(new Velvet::ImGuiLayer());
	}
	~WindowApp()
	{

	}
};

Velvet::Application* Velvet::CreateApplication()
{
	VL_WARN("Creating Application...");
	return new WindowApp;
}