#include "axpch.h"
#include "CameraData.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Axton
{
	void CameraData::RecalculateViewMatrix(Vector3 position)
	{
		ViewMatrix = glm::translate(Matrix4(1.0f), position);
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
}