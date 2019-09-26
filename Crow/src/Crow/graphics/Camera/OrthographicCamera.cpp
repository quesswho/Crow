#include "OrthographicCamera.h"
#include "Crow/Event/Input.h"

namespace Crow {

	OrthographicCamera::OrthographicCamera(glm::vec3& pos, float x, float y, float speed, float zoomSpeed)
		: m_CameraPos(pos), m_ProjectionMatrix(glm::ortho(-x, x, -y, y, 0.0f, 100.0f)), m_Speed(speed), m_Zoom(x, y), m_ZoomSpeed(zoomSpeed)
	{
		CalculateMatrices();
	}

	OrthographicCamera::~OrthographicCamera()
	{}

	void OrthographicCamera::CalculateMatrices()
	{
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_CameraPos);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	glm::vec3& OrthographicCamera::Update(float elapsed)
	{
		if (Input::IsKeyPressed(CROW_KEY_W))
			m_CameraPos += glm::vec3(0.0f,	-m_Speed * elapsed, 0.0f);
		if (Input::IsKeyPressed(CROW_KEY_S))
			m_CameraPos += glm::vec3(0.0f, m_Speed * elapsed, 0.0f);
		if (Input::IsKeyPressed(CROW_KEY_A))
			m_CameraPos += glm::vec3(m_Speed * elapsed, 0.0f, 0.0f);
		if (Input::IsKeyPressed(CROW_KEY_D))
			m_CameraPos += glm::vec3(-m_Speed * elapsed, 0.0f, 0.0f);
		
		if (Input::IsKeyPressed(CROW_KEY_Z))
		{
			m_Zoom -= glm::vec2(m_ZoomSpeed * elapsed, m_ZoomSpeed * elapsed);
			m_ProjectionMatrix = glm::ortho(-m_Zoom.x, m_Zoom.x, -m_Zoom.y, m_Zoom.y, 0.0f, 100.0f);
		}
		if (Input::IsKeyPressed(CROW_KEY_X))
		{
			m_Zoom += glm::vec2(m_ZoomSpeed * elapsed, m_ZoomSpeed * elapsed);
			m_ProjectionMatrix = glm::ortho(-m_Zoom.x, m_Zoom.x, -m_Zoom.y, m_Zoom.y, 0.0f, 100.0f);
		}

		CalculateMatrices();
		return m_CameraPos;
	}
}