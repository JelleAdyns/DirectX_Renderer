#pragma once
#include <cassert>
#include <SDL_keyboard.h>
#include <SDL_mouse.h>
#include <iostream>

#include "Math.h"
#include "Timer.h"

namespace dae
{
	class Camera final
	{
	public:
		Camera() = default;

		Camera(const Vector3& _origin, float _fovAngle);

		void Initialize(int _width, int _height, float _fovAngle = 90.f, Vector3 _origin = { 0.f,0.f,0.f });

		void CalculateViewMatrix();
		void CalculateProjectionMatrix();

		void Update(const Timer* pTimer);

		void HandleKeyMovement(const uint8_t* keys, float elapsedSec);
		void TransformForwardVector();


		Matrix GetViewMatrix() const { return m_ViewMatrix; }
		Matrix GetProjectionMatrix() const { return m_ProjectionMatrix; }
	private:
		const float m_Near{ 1.f };
		const float m_Far{ 100.f };

		Vector3 m_Origin{};
		float m_FovAngle{ 90.f };
		float m_Fov{ tanf((m_FovAngle * TO_RADIANS) / 2.f) };

		Vector3 m_Forward{ Vector3::UnitZ };
		Vector3 m_Up{ Vector3::UnitY };
		Vector3 m_Right{ Vector3::UnitX };

		float m_TotalPitch{};
		float m_TotalYaw{};

		const float m_TranslateSpeed{ 10.f };

		Matrix m_InvViewMatrix{};
		Matrix m_ViewMatrix{};
		Matrix m_ProjectionMatrix{};

		int m_width{};
		int m_Height{};
	};
}


