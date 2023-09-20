#pragma once
#include "Axton/Core/Defines.h"

#include <filesystem>

namespace Axton
{
	namespace FS = std::filesystem;

	class AX_API FileSystem
	{
	public:
		FileSystem(const char* path);
		FileSystem(const std::string& path);

		std::string ReadFile();

	private:
		FS::path m_InternalPath;
	};
}