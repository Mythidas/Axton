#pragma once

#include "Axton.h"
#include "World.h"
#include "OctreeChunk.h"

using namespace Axton;

class VoxModelLoader
{
public:
	VoxModelLoader(const std::string& path);

	Ref<OctreeChunk> GenChunk(World& world, Vector3 position);

private:
	void FindValueIndex(const std::vector<uint8_t>& buffer, uint32_t value, size_t& currentIndex);
	uint32_t GetNextValue(const std::vector<uint8_t>& buffer, size_t& currentIndex);

private:
	std::string m_Path;
};