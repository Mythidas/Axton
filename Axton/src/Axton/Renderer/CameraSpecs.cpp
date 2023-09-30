#include "axpch.h"
#include "CameraSpecs.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Axton
{
	void CameraSpecs::RecalculateViewMatrix(Vector3 position, Vector3 forward, Vector3 right)
	{
		ViewMatrix = glm::lookAt(position, position + forward, right);
	}

	void CameraSpecs::RecalculateProjectionMatrix()
	{
		if (Format == ProjectionFormat::Orthographic)
		{
			ProjectionMatrix = glm::ortho(-OrthoSize * AspectRatio, OrthoSize * AspectRatio, 
				-OrthoSize, OrthoSize, 
				NearClip, FarClip);
		}
		else if (Format == ProjectionFormat::Perspective)
		{
			ProjectionMatrix = glm::perspective(glm::radians(FOV), AspectRatio,
				NearClip, FarClip);
		}
	}

	Matrix4 CameraSpecs::GetViewProjectionMatrix() const
	{
		return ProjectionMatrix * ViewMatrix;
	}
}