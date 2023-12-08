#pragma once

#include <Velvet.h>

class Sandbox2D : public Velvet::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Velvet::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Velvet::Event& e) override;
private:
	Velvet::OrthographicCameraController m_CameraController;

	// Temporary
	Velvet::Ref<Velvet::VertexArray> m_SquareVAO;
	Velvet::Ref<Velvet::Shader> m_FlatColorShader;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};