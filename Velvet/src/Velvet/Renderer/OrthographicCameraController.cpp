#include <vlpch.h>
#include "OrthographicCameraController.h"

#include "Velvet/Core/Input.h"
#include "Velvet/Core/KeyCodes.h"

namespace Velvet {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		float timedCameraSpeed = m_CameraTranslationSpeed * ts;
		if (Input::IsKeyPressed(VL_KEY_A))
		{
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * timedCameraSpeed;
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * timedCameraSpeed;
		}
		else if (Velvet::Input::IsKeyPressed(VL_KEY_D))
		{
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * timedCameraSpeed;
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * timedCameraSpeed;
		}

		if (Input::IsKeyPressed(VL_KEY_W))
		{
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * timedCameraSpeed;
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * timedCameraSpeed;
		}
		else if (Input::IsKeyPressed(VL_KEY_S))
		{
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * timedCameraSpeed;
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * timedCameraSpeed;
		}

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(VL_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			else if (Input::IsKeyPressed(VL_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(VL_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(VL_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.2f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}