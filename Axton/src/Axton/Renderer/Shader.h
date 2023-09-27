#pragma once

#include "Axton/Core/Defines.h"
#include "Axton/Math/Math.h"

namespace Axton
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void LoadShader() = 0;

		virtual void SetMat4(const std::string& name, const Matrix4& value) = 0;
		virtual void SetInt(const std::string& name, const int value) = 0;
		virtual void SetIntArray(const std::string& name, const int value[], size_t size) = 0;

		static Ref<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc, bool lateLoad);
	};
}