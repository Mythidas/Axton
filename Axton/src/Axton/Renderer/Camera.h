#pragma once

#include "Axton/Math/Math.h"

namespace Axton
{
	class Camera
	{
	public:
		struct Specs
		{
			float VFov = 45;
			float NearClip = 0.1f;
			float FarClip = 100.0f;
		};

	public:
		virtual ~Camera() = default;

		virtual void OnResize(uint32_t width, uint32_t height) = 0;
		virtual void RecalculateProjection() = 0;
		virtual void RecalculateView() = 0;

		virtual const Matrix4& GetProjection() const { return m_Projection; }
		virtual const Matrix4& GetInverseProjection() const { return m_InverseProjection; }
		virtual const Matrix4& GetView() const { return m_View; }
		virtual const Matrix4& GetInverseView() const { return m_InverseView; }

		virtual Vector3 GetPosition() const { return m_Position; }
		virtual Vector3 GetDirection() const { return m_Direction; }

	protected:
		Specs m_Specs;

		Matrix4 m_Projection{ 1.0f };
		Matrix4 m_View{ 1.0f };
		Matrix4 m_InverseProjection{ 1.0f };
		Matrix4 m_InverseView{ 1.0f };

		Vector3 m_Position{ 0.0f };
		Vector3 m_Direction{ 0.0f };
	};
}