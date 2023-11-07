#pragma once

#include "Velvet/Core/Core.h"

#include "Velvet/Core/Window.h"
#include "Velvet/Core/LayerStack.h"
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

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}