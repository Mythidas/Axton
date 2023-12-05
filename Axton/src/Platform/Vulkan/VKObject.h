#pragma once

#include "Axton/Utils/UUID.h"

namespace Axton::Vulkan
{
	class VKObject
	{
	public:
		VKObject();
		virtual ~VKObject();

		virtual void Destroy() = 0;

		bool operator==(const VKObject& rhs) const
		{
			return m_UUID == rhs.m_UUID;
		}

	private:
		UUID m_UUID;
	};
}