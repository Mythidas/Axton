#pragma once

#include "Axton.h"
#include "Material.h"
#include "Chunk.h"

using namespace Axton;

class World
{
public:
	World(uint32_t maxVoxels);

	Ref<Chunk> CreateChunk(Vector3 position, IVector3 extents, bool sparse);
	size_t AddMaterial(Ref<Material> material);

	void BeginEdit(Ref<Chunk> chunk);
	void EndEdit(Ref<Chunk> chunk);

	void LoadBuffers(const Camera& camera);

	std::vector<Ref<Material>> Materials() { return m_Materials; }

private:
	std::vector<Ref<Chunk>> m_Chunks;
	std::vector<Ref<Material>> m_Materials;

	Ref<StorageBuffer> m_VoxelStorage;
	Ref<StorageBuffer> m_ChunkStorage;
	Ref<StorageBuffer> m_MaterialStorage;
};