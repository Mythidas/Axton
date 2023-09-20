#include "axpch.h"
#include "FileSystem.h"

namespace Axton
{
	FileSystem::FileSystem(const char* path)
	{
		m_InternalPath = path;
	}

	FileSystem::FileSystem(const std::string& path)
	{
		m_InternalPath = path;
	}

	std::string FileSystem::ReadFile()
	{
		std::ifstream stream(m_InternalPath.c_str());
		std::stringstream buffer;
		buffer << stream.rdbuf();

		return buffer.str();
	}
}