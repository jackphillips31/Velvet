#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"

#include "Velvet/Events/Event.h"
#include "Velvet/Events/ApplicationEvent.h"

#include "Velvet/ImGui/ImGuiLayer.h"

#include "Velvet/Renderer/Shader.h"
#include "Velvet/Renderer/Buffer.h"
#include "Velvet/Renderer/VertexArray.h"
#include "Velvet/Renderer/OrthographicCamera.h"

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
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_TriangleVA;

		std::shared_ptr<Shader> m_BlueShader;
		std::shared_ptr<VertexArray> m_SquareVA;

		OrthographicCamera m_Camera;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}