#include "pch.h"
#include "Camera.h"

namespace dae
{
	Camera::Camera(const Vector3& _origin, float _fovAngle) :
		m_Origin{ _origin },
		m_FovAngle{ _fovAngle }
	{

	}

	void Camera::Initialize(int _width, int _height, float _fovAngle, Vector3 _origin)
	{
		m_FovAngle = _fovAngle;
		m_Fov = tanf((m_FovAngle * TO_RADIANS) / 2.f);

		m_Origin = _origin;

		m_width = _width;
		m_Height = _height;
		CalculateViewMatrix();
		CalculateProjectionMatrix();
	}

	void Camera::CalculateViewMatrix()
	{
		m_Right = Vector3::Cross(Vector3::UnitY, m_Forward).Normalized();
		m_Up = Vector3::Cross(m_Forward, m_Right).Normalized();

		Matrix invView{ {m_Right, 0},
						{m_Up, 0},
						{m_Forward, 0},
						{m_Origin, 1} };
		m_InvViewMatrix = invView;
		m_ViewMatrix = Matrix::Inverse(invView);
	}

	void Camera::CalculateProjectionMatrix()
	{
		m_ProjectionMatrix = { {1 / (float(m_width) / m_Height * m_Fov),0,0,0},
							 {0, 1 / m_Fov, 0, 0},
							 {0, 0, m_Far / (m_Far - m_Near), 1},
							 {0, 0, -(m_Far * m_Near) / (m_Far - m_Near), 0} };
	}

	void Camera::Update(const Timer* pTimer)
	{
		const float deltaTime = pTimer->GetElapsed();
		//Keyboard Input
		const uint8_t* pKeyboardState = SDL_GetKeyboardState(nullptr);
		//Mouse Input
		int mouseX{}, mouseY{};
		const uint32_t mouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);

		HandleKeyMovement(pKeyboardState, deltaTime);

		if (mouseState == SDL_BUTTON(SDL_BUTTON_RIGHT))
		{
			if (mouseY != 0)
			{
				m_TotalPitch -= mouseY;
				if (m_TotalPitch >= 89) m_TotalPitch = 89;
				if (m_TotalPitch <= -89) m_TotalPitch = -89;
				TransformForwardVector();
			}
			if (mouseX != 0)
			{
				m_TotalYaw += mouseX;
				TransformForwardVector();
			}
		}

		if (mouseState == SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			if (mouseY != 0) m_Origin += m_Forward * float(-mouseY) * m_TranslateSpeed * deltaTime * 10.f;

			if (mouseX != 0)
			{
				m_TotalYaw += mouseX;
				TransformForwardVector();
			}
		}
		if (mouseState == (SDL_BUTTON(SDL_BUTTON_LEFT) | SDL_BUTTON(SDL_BUTTON_RIGHT)))
		{
			if (mouseY != 0) m_Origin.y += float(mouseY) * m_TranslateSpeed * deltaTime * 10.f;
		}

	}

	void Camera::HandleKeyMovement(const uint8_t* keys, float elapsedSec)
	{
		if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])
		{
			m_Origin += m_Forward * m_TranslateSpeed * elapsedSec;
		}
		if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])
		{
			m_Origin -= m_Forward * m_TranslateSpeed * elapsedSec;
		}
		if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT])
		{
			m_Origin += m_Right * m_TranslateSpeed * elapsedSec;
		}
		if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
		{
			m_Origin -= m_Right * m_TranslateSpeed * elapsedSec;
		}
		if (keys[SDL_SCANCODE_E])
		{
			m_Origin.y += m_TranslateSpeed * elapsedSec;
		}
		if (keys[SDL_SCANCODE_Q])
		{
			m_Origin.y -= m_TranslateSpeed * elapsedSec;
		}
		if (keys[SDL_SCANCODE_Z])
		{
			m_FovAngle += m_TranslateSpeed * elapsedSec;
			if (m_FovAngle > 360) m_FovAngle = 360;
			m_Fov = tan(TO_RADIANS * m_FovAngle / 2);
			CalculateProjectionMatrix();
			std::cout << m_FovAngle << std::endl;
		}
		if (keys[SDL_SCANCODE_C])
		{
			m_FovAngle -= m_TranslateSpeed * elapsedSec;
			if (m_FovAngle < 0) m_FovAngle = 0;
			m_Fov = tan(TO_RADIANS * m_FovAngle / 2);
			CalculateProjectionMatrix();
			std::cout << m_FovAngle << std::endl;
		}

		//Update Matrices
		CalculateViewMatrix();
	}
	void Camera::TransformForwardVector()
	{
		Matrix finalRotation{ Matrix::CreateRotation(m_TotalPitch * TO_RADIANS, m_TotalYaw * TO_RADIANS, 0) };

		m_Forward = finalRotation.TransformVector(Vector3::UnitZ).Normalized();
		CalculateViewMatrix();
	}
}
