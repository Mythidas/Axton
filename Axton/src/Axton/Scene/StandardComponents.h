#pragma once

#include "Component.h"
#include "Axton/Renderer/CameraData.h"
#include "Axton/Core/Defines.h"
#include "Axton/Math/Math.h"
#include "Axton/Renderer/Sprite.h"

namespace Axton
{
	class Tag : public ECSComponent
	{
	public:
		std::string Name{ "" };
	};

	class Transform : public ECSComponent
	{
	public:
		Vector3 Position{ 0.0f, 0.0f, 0.0f };
		Vector3 Scale{ 1.0f, 1.0f, 1.0f };
		Vector3 Rotation{ 0.0f, 0.0f, 0.0f };
	};

	class Camera : public ECSComponent
	{
	public:
		CameraData Data{};
	};

	class SpriteRenderer : public ECSComponent
	{
	public:
		Vector4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Sprite> Sprite{ nullptr };
	};
}