#include <Velvet.h>

class WindowApp : public Velvet::Application
{
public:
	WindowApp()
	{

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