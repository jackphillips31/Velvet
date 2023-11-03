#pragma once

#include "Velvet/Core.h"

#include "Velvet/Window.h"
#include "Velvet/LayerStack.h"
#include "Velvet/Events/Event.h"
#include "Velvet/Events/ApplicationEvent.h"

namespace Velvet {

	class VELVET_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}