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

	std::vector<unsigned char> FileSystem::ToBuffer()
	{
		std::ifstream stream(m_InternalPath.c_str(), std::ios::binary);
		if (!stream.is_open())
		{
			CoreLog::Warn("Failed to open file {0}", m_InternalPath.string());
			return std::vector<unsigned char>();
		}

		std::vector<unsigned char> buffer;
		unsigned char value = 0;
		while (stream.read(reinterpret_cast<char*>(&value), sizeof(value)))
		{
			buffer.push_back(value);
		}

		stream.close();
		return buffer;
	}

	std::vector<char> FileSystem::ToSignedBuffer()
	{
		std::ifstream stream(m_InternalPath.c_str(), std::ios::binary);
		if (!stream.is_open())
		{
			CoreLog::Warn("Failed to open file {0}", m_InternalPath.string());
			return std::vector<char>();
		}

		std::vector<char> buffer;
		char value = 0;
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