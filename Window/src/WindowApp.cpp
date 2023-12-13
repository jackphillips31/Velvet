#include <Velvet.h>
#include <Velvet/Core/EntryPoint.h>

#include "Sandbox2D.h"

class WindowApp : public Velvet::Application
{
public:
	WindowApp()
	{
		PushLayer(new Sandbox2D());
	}
	~WindowApp()
	{

	}
};

Velvet::Application* Velvet::CreateApplication()
{
	return new WindowApp;
}