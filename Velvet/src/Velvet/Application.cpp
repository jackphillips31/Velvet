#include "vlpch.h"
#include "Application.h"

#include "Velvet/Events/ApplicationEvent.h"

namespace Velvet {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		VL_CORE_TRACE("Running application...");
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}

}