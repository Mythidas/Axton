#pragma once
#include "Axton/Core/Defines.h"

#include <filesystem>

namespace Axton
{
	namespace FS = std::filesystem;

	class FileSystem
	{
	public:
		FileSystem() = default;
		FileSystem(const char* path);
		FileSystem(const std::string& path);

		std::string StreamString();
		std::vector<unsigned char> StreamBuffer();
		std::vector<char> StreamSignedBuffer();

		std::string ToString() const { return m_InternalPath.string(); }

	private:
		std::ifstream getStream(bool binary = false);

	private:
		FS::path m_InternalPath;
	};
}