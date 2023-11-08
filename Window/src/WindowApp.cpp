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
		if (Velvet::Input::IsKeyPressed(VL_KEY_TAB))
			VL_TRACE("Tab key is pressed! (poll)");
	}

	void OnEvent(Velvet::Event& event) override
	{
		if (event.GetEventType() == Velvet::EventType::KeyPressed)
		{
			Velvet::KeyPressedEvent& e = (Velvet::KeyPressedEvent&)event;
			if (e.GetKeyCode() == VL_KEY_TAB)
				VL_TRACE("Tab key is pressed! (event)");
			VL_TRACE((char)e.GetKeyCode());
		}
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