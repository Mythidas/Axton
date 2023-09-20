#pragma once

#include "Axton/Core/Defines.h"

namespace Axton
{
	class AX_API Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void LoadShader() = 0;

		static Ref<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc, bool lateLoad);
	};
}