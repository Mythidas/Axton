#pragma once

#include "Axton/Core/Defines.h"

namespace Axton
{
	class AX_API Component
	{
	public:
		static std::string GetBaseType() { return GetTypeName<Component>(); }
	};

	class ECSComponent : public Component
	{
	public:
		static std::string GetBaseType() { return GetTypeName<ECSComponent>(); }
	};

	class NativeScriptComponent : public Component
	{
	public:
		NativeScriptComponent() = default;
		virtual ~NativeScriptComponent() = default;

		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate() {}
		virtual void OnFixedUpdate() {}
		virtual void OnRenderUI() {}

		static std::string GetBaseType() { return GetTypeName<NativeScriptComponent>(); }
	};
}