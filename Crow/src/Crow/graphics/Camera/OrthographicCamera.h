#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Crow {

	class OrthographicCamera {
	private:
		glm::mat4x4 m_ViewMatrix;
		glm::mat4x4 m_ProjectionMatrix;
		glm::mat4x4 m_ProjectionViewMatrix;

		glm::vec3	m_CameraPos;
		glm::vec2   m_Zoom;

		float m_Speed;
		float m_ZoomSpeed;
	public:
		OrthographicCamera(glm::vec3& pos, float x, float y, float speed, float zoomSpeed);
		OrthographicCamera(glm::vec2& pos, float x, float y, float speed = 1.0f)
			: OrthographicCamera(glm::vec3(pos, 0.0f), x, y, speed, 0.6f)
		{}

		OrthographicCamera(glm::vec3& pos, float x, float y, float speed = 1.0f)
			: OrthographicCamera(pos, x, y, speed, 0.6f)
		{}
		OrthographicCamera(glm::vec2& pos, float x, float y, float speed, float zoomSpeed)
			: OrthographicCamera(glm::vec3(pos, 0.0f), x, y, speed, zoomSpeed)
		{}


		~OrthographicCamera();

		void Update(float elapsed);

		const glm::mat4x4& GetViewMatrix() const { return m_ViewMatrix; }
		void SetViewMatrix(const glm::mat4x4& view) noexcept { m_ViewMatrix = view; CalculateMatrices(); }
		const glm::mat4x4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		void SetProjectionMatrix(const glm::mat4x4& projection) noexcept { m_ProjectionMatrix = projection; CalculateMatrices(); }

		const glm::mat4x4& GetProjectionViewMatrix() const { return m_ProjectionViewMatrix; }

		const glm::vec3& GetCameraPos() const { return m_CameraPos; }
		void SetCameraPos(const glm::vec3& pos) noexcept { m_CameraPos = pos; CalculateMatrices(); }

	private:
		void CalculateMatrices();
	};
}