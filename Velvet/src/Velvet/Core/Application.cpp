#include <vlpch.h>
#include "Velvet/Core/Application.h"

#include "Velvet/Core/Input.h"

#include "Velvet/Renderer/Renderer.h"
#include "Velvet/Renderer/UIController.h"

#include <glfw/glfw3.h>

namespace Velvet {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		VL_PROFILE_FUNCTION();

		VL_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		VL_CORE_INFO("Constructing Application");

		m_Window = Window::Create();
		m_Window->SetEventCallback(VL_BIND_EVENT_FN(Application::OnEvent));

		Input::Init();
		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		VL_PROFILE_FUNCTION();

		Renderer::Shutdown();
		ResourceManager::Shutdown();
		Input::Shutdown();
		m_Window.reset();
	}

	void Application::PushLayer(Layer* layer)
	{
		VL_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		VL_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Run()
	{
		VL_PROFILE_FUNCTION();

		VL_CORE_INFO("Running Application...");
		while (m_Running)
		{
			VL_PROFILE_SCOPE("RunLoop");

			float time = (float)glfwGetTime(); // TODO: Platform::GetTime()
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				VL_PROFILE_SCOPE("LayerStack OnUpdate");

				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}

			m_ImGuiLayer->Begin();
			{
				VL_PROFILE_SCOPE("LayerStack OnImGuiRender");

				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		VL_PROFILE_FUNCTION();

		UIController::OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(VL_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(VL_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			(*it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		VL_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

}