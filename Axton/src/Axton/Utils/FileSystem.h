#pragma once
#include "Axton/Core/Defines.h"

#include <filesystem>

namespace Axton
{
	namespace FS = std::filesystem;

	class FileSystem
	{
	public:
		FileSystem(const char* path);
		FileSystem(const std::string& path);

		std::string ToString();
		std::vector<unsigned char> ToBuffer();
		std::vector<char> ToSignedBuffer();

	private:
		FS::path m_InternalPath;
	};
}