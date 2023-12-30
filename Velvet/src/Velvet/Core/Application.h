#pragma once

#include "Velvet/Core/Core.h"

#include "Velvet/Core/Window.h"
#include "Velvet/Core/LayerStack.h"
#include "Velvet/Events/Event.h"
#include "Velvet/Events/ApplicationEvent.h"

#include "Velvet/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Velvet {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }
	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}