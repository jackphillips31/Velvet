#include "vlpch.h"
#include "Application.h"

#include "Velvet/Events/ApplicationEvent.h"
#include "Velvet/Log.h"

namespace Velvet {

	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		VL_CORE_TRACE("Running application...");
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication))
		{
			VL_CORE_INFO(e);
		}
		else if (e.IsInCategory(EventCategoryInput))
		{
			VL_CORE_INFO(e);
		}
		while (true);
	}

}