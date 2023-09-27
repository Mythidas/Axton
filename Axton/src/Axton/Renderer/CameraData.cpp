#include "axpch.h"
#include "CameraData.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Axton
{
	void CameraData::RecalculateViewMatrix(Vector3 position, Vector3 forward, Vector3 right)
	{
		ViewMatrix = glm::lookAt(position, position + forward, right);
	}

	void CameraData::RecalculateProjectionMatrix()
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

	Matrix4 CameraData::GetViewProjectionMatrix() const
	{
		return ProjectionMatrix * ViewMatrix;
	}
}