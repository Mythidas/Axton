#pragma once

#include "Axton/Core/Defines.h"

#include <functional>

namespace Axton
{
	template <typename... Args>
	class Event
	{
		typedef std::function<bool(Args...)> Func;
	public:
		void operator()(Args... args)
		{
			for (auto& callback : m_Callbacks)
			{
				if (callback(args...))
					break;
			}
		}

		void operator +=(const Func& callback)
		{
			m_Callbacks.push_back(callback);
		}

	private:
		std::vector<Func> m_Callbacks;
	};
}