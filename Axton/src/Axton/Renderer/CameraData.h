#pragma once
#include "Axton/Core/Defines.h"
#include "Axton/Math/Math.h"

namespace Axton
{
	struct CameraData
	{
		enum class ProjectionFormat
		{
			Perspective = 0,
			Orthographic = 1
		};

		ProjectionFormat Format = ProjectionFormat::Orthographic;
		float FOV = 45.0f;
		float NearClip = 0.1f;
		float FarClip = 100.0f;
		float AspectRatio = 16.0f / 9.0f;
		float OrthoSize = 2.0f;
		Matrix4 ViewMatrix;
		Matrix4 ProjectionMatrix;

		void RecalculateViewMatrix(Vector3 position);
		void RecalculateProjectionMatrix();
	};
}