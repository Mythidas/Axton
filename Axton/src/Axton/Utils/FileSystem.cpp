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

	std::vector<uint8_t> FileSystem::ToBuffer()
	{
		std::ifstream stream(m_InternalPath.c_str(), std::ios::binary);
		if (!stream.is_open())
		{
			CoreLog::Warn("Failed to open file {0}", m_InternalPath.string());
			return std::vector<uint8_t>();
		}

		std::vector<uint8_t> buffer;
		uint8_t value = 0;
		while (stream.read(reinterpret_cast<char*>(&value), sizeof(value)))
		{
			buffer.push_back(value);
		}

		stream.close();
		return buffer;
	}

	std::string FileSystem::ToString()
	{
		std::ifstream stream(m_InternalPath.c_str());
		if (!stream.is_open())
		{
			CoreLog::Warn("Failed to open file {0}", m_InternalPath.string());
			return std::string();
		}

		std::stringstream buffer;
		buffer << stream.rdbuf();
		stream.close();
		return buffer.str();
	}
}