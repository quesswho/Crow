#include "FirstPersonCamera.h"
#include "Crow/Event/Input.h"
#include "Crow/Application.h"
#include "Crow/Common.h"

namespace Crow {
	namespace Math {


		FirstPersonCamera::FirstPersonCamera(Vec3& pos, Vec3& viewDir, float aspectRatio, float sensitivity, float speed)
			: m_CameraPos(pos), m_ViewDir(viewDir), m_Yaw(90.0f), m_Pitch(0.0f), m_Sensitivity(sensitivity), m_Speed(speed), m_Forward(0.0f, 0.0f, 0.0f), m_Right(1.0f, 0.0f, 0.0f), m_WorldUp(Vec3(0.0f, 1.0f, 0.0f))
		{
			m_ProjectionMatrix = Mat4::Perspective(45.0f, aspectRatio, 0.001f, 100.0f);
			m_LastMousePos = TVec2<int>(Application::s_WindowProperties.m_Width / 2, Application::s_WindowProperties.m_Height / 2);
			Application::SetCursorPosition(m_LastMousePos);
			Application::SetCursorVisibility(false);
			CalculateProjectionViewMatrix();
			m_MouseRect = TVec4<int>(Application::s_WindowProperties.m_Width / 4, (int)Application::s_WindowProperties.m_Width * (3.0 / 4.0), Application::s_WindowProperties.m_Height / 4, (int)Application::s_WindowProperties.m_Height * (3.0 / 4.0));
		}

		void FirstPersonCamera::Update(float elapsed)
		{
			if (Input::IsKeyPressed(CROW_KEY_W))
				m_CameraPos += m_Forward * m_Speed * elapsed;
			if (Input::IsKeyPressed(CROW_KEY_S))
				m_CameraPos -= m_Forward * m_Speed * elapsed;
			if (Input::IsKeyPressed(CROW_KEY_D))
				m_CameraPos += m_Right * m_Speed * elapsed;
			if (Input::IsKeyPressed(CROW_KEY_A))
				m_CameraPos -= m_Right * m_Speed * elapsed;

			if (Input::IsKeyPressed(CROW_KEY_SPACE))
				m_CameraPos += m_Up * m_Speed * elapsed;
			if (Input::IsKeyPressed(CROW_KEY_LEFT_CONTROL))
				m_CameraPos -= m_Up * m_Speed * elapsed;

			CalculateProjectionViewMatrix();
		}

		void FirstPersonCamera::UpdateRotation()
		{
			if (Input::s_Focused)
			{

				float movementX = (float) Input::GetMousePosition().x - m_LastMousePos.x;
				float movementY = (float) m_LastMousePos.y - Input::GetMousePosition().y;

				if (m_LastMousePos.x < m_MouseRect.x || m_LastMousePos.x > m_MouseRect.y || m_LastMousePos.y < m_MouseRect.z || m_LastMousePos.y > m_MouseRect.w)
				{
					m_LastMousePos = TVec2<int>(Application::s_WindowProperties.m_Width / 2, Application::s_WindowProperties.m_Height / 2);
					Application::SetCursorPosition(m_LastMousePos);
				}
				else
					m_LastMousePos = Input::GetMousePosition();



				movementX *= m_Sensitivity;
				movementY *= m_Sensitivity;

				m_Yaw = std::fmod((m_Yaw + movementX), 360.0f); // Prevent yaw from reaching high numbers
				m_Pitch += movementY;

				if (m_Pitch > 89.0f)
					m_Pitch = 89.0f;
				if (m_Pitch < -89.0f)
					m_Pitch = -89.0f;

				CalculateProjectionViewMatrix();
			}
		}

		void FirstPersonCamera::CalculateProjectionViewMatrix()
		{
			m_Forward = Normalize(Vec3(cos(ToRadians(m_Yaw)), 0.0f, sin(ToRadians(m_Yaw))));
			m_ViewDir = Normalize(Vec3(cos(ToRadians(m_Yaw)) * cos(ToRadians(m_Pitch)), sin(ToRadians(m_Pitch)), sin(ToRadians(m_Yaw)) * cos(ToRadians(m_Pitch))));
			m_Right = Normalize(Cross(m_Forward, m_WorldUp));
			m_Up = Normalize(Cross(m_Right, m_Forward));
			m_ViewMatrix = Mat4::LookDir(m_CameraPos, m_ViewDir, m_Up);
			if (MATH_COORDINATE::s_MathCoordinateType == MATH_COORDINATE::MATH_COORDINATE_RIGHTHAND)
				m_CameraMatrix = m_ProjectionMatrix * m_ViewMatrix; // Opengl
			else
				m_CameraMatrix = m_ViewMatrix * m_ProjectionMatrix; // DirectX
		}
	}
}
