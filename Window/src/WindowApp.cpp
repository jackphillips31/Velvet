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
	return new WindowApp;
}