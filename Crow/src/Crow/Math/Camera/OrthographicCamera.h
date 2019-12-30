#pragma once

#include "Crow/Math/Maths.h"

namespace Crow {

	using namespace Math;

	class OrthographicCamera {
	private:
		Mat4 m_ViewMatrix;
		Mat4 m_ProjectionMatrix;
		Mat4 m_ProjectionViewMatrix;

		Vec3 m_CameraPos;
		Vec2 m_Zoom;

		float m_Speed;
		float m_ZoomSpeed;
	public:
		OrthographicCamera(Vec3& pos, float x, float y, float speed, float zoomSpeed);
		OrthographicCamera(Vec2& pos, float x, float y, float speed = 1.0f)
			: OrthographicCamera(Vec3(pos, 0.0f), x, y, speed, 0.6f)
		{}

		OrthographicCamera(Vec3& pos, float x, float y, float speed = 1.0f)
			: OrthographicCamera(pos, x, y, speed, 0.6f)
		{}
		OrthographicCamera(Vec2& pos, float x, float y, float speed, float zoomSpeed)
			: OrthographicCamera(Vec3(pos, 0.0f), x, y, speed, zoomSpeed)
		{}


		~OrthographicCamera();

		Vec3& Update(float elapsed);

		const Mat4& GetViewMatrix() const { return m_ViewMatrix; }
		void SetViewMatrix(const Mat4& view) noexcept { m_ViewMatrix = view; CalculateMatrices(); }
		const Mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		void SetProjectionMatrix(const Mat4& projection) noexcept { m_ProjectionMatrix = projection; CalculateMatrices(); }

		const Mat4& GetProjectionViewMatrix() const { return m_ProjectionViewMatrix; }

		const Vec3& GetCameraPos() const { return m_CameraPos; }
		void SetCameraPos(const Vec3& pos) noexcept { m_CameraPos = pos; CalculateMatrices(); }

	private:
		void CalculateMatrices();
	};
}