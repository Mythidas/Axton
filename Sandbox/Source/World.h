#pragma once

#include "Axton.h"
#include "Material.h"
#include "OctreeChunk.h"

using namespace Axton;

class World
{
public:
	World(uint32_t maxVoxels);

	Ref<OctreeChunk> CreateChunk(Vector3 position, IVector3 extents);
	size_t AddMaterial(Ref<Material> material);

	void LoadBuffers(const Camera& camera);

	std::vector<Ref<Material>> Materials() { return m_Materials; }

private:
	std::vector<Ref<OctreeChunk>> m_Chunks;
	std::vector<Ref<Material>> m_Materials;

	Ref<StorageBuffer> m_VoxelStorage;
	Ref<StorageBuffer> m_ChunkStorage;
	Ref<StorageBuffer> m_MaterialStorage;
};