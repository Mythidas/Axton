#pragma once

#include "Axton/Core/Defines.h"
#include "Axton/Math/Math.h"

namespace Axton
{
	class Transform;

	struct CameraData
	{
		enum class ProjectionFormat
		{
			Perspective = 0,
			Orthographic = 1
		};

		ProjectionFormat Format = ProjectionFormat::Perspective;
		float FOV = 60.0f;
		float MaxFOV = 120.0f;
		float NearClip = 0.01f;
		float FarClip = 100.0f;
		float AspectRatio = 16.0f / 9.0f;
		float OrthoSize = 2.0f;
		Matrix4 ViewMatrix;
		Matrix4 ProjectionMatrix;

		void RecalculateViewMatrix(Vector3 position, Vector3 forward, Vector3 right);
		void RecalculateProjectionMatrix();
		Matrix4 GetViewProjectionMatrix() const;
	};
}