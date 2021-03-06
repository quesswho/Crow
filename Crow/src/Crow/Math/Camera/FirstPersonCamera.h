#pragma once

#include "Crow/Math/Maths.h"

namespace Crow {
	namespace Math {

		class FirstPersonCamera {
		private:
			Mat4 m_ViewMatrix;
			Mat4 m_ProjectionMatrix;
			Mat4 m_CameraMatrix;

			Vec3 m_CameraPos;
			Vec3 m_ViewDir;
			Vec3 m_Forward;
			Vec3 m_Right;
			Vec3 m_Up;
			Vec3 m_WorldUp;

			float m_Yaw, m_Pitch;
			float m_Speed;
			float m_Sensitivity;

			TVec2<double> m_LastMousePos;
			TVec4<int> m_MouseRect;

		public:
			FirstPersonCamera(Vec3& pos, Vec3& viewDir, float aspectRatio, float sensitivity, float speed);

			FirstPersonCamera(Vec3& pos, float aspectRatio, float sensitivity, float speed = 1.0f)
				: FirstPersonCamera(pos, Vec3(0.0f, 0.0f, 1.0f), aspectRatio, sensitivity, speed)
			{}


			~FirstPersonCamera() {}

			void Update(float elapsed);
			void UpdateRotation();


			inline const Mat4& GetViewMatrix() const { return m_ViewMatrix; }

			inline const Mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
			void SetProjectionMatrix(const Mat4& projection) noexcept { m_ProjectionMatrix = projection; CalculateProjectionViewMatrix(); }

			inline const Vec3& GetCameraPos() const { return m_CameraPos; }
			void SetCameraPos(const Vec3& pos) noexcept { m_CameraPos = pos; CalculateProjectionViewMatrix(); }

			inline const Vec3& GetViewDir() const { return m_ViewDir; }
			void SetViewDir(const Vec3& dir) noexcept { m_ViewDir = dir; CalculateProjectionViewMatrix(); }

			// Yaw, Pitch
			void SetRotation(const Vec2& rotation) noexcept { m_Yaw = rotation.x; m_Pitch = rotation.y; }

			inline const Mat4& GetCameraMatrix() const { return m_CameraMatrix; }

		private:
			inline void CalculateProjectionViewMatrix();
		};
	}
}