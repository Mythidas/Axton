#pragma once

#include <memory>
#include <functional>
#include <string>

#ifdef AX_PLAT_WINDOWS
	#ifdef AX_BUILD_DLL
		#define AX_API __declspec(dllexport)
	#else
		#define AX_API __declspec(dllimport)
	#endif
#endif

#define AX_BIND_FNC(fn)  [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Axton
{
	// Scope for unique pointers
	template <typename T>
	using Scope = std::unique_ptr<T>;

	template <typename T, typename... Args>
	Scope<T> CreateScope(Args ...args)
	{
		return std::make_unique<T>(args...);
	}

	// Reference for shared pointers
	template <typename T>
	using Ref = std::shared_ptr<T>;

	template <typename T, typename ...Args>
	Ref<T> CreateRef(Args... args)
	{
		return std::make_shared<T>(args...);
	}

	// Weak reference for weak pointers
	template <typename T>
	using WeakRef = std::weak_ptr<T>;

	template <typename... Args>
	using Function = std::function<void(Args...)>;

	// Type Traverse
	template <typename T> 
	std::string GetTypeName()
	{
		typedef typename std::remove_reference<T>::type TR;
		return typeid(TR).name();
	}
}