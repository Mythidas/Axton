#pragma once

#include "Axton.h"
#include "Chunk.h"

using namespace Axton;

class World
{
public:
	World(uint32_t maxVoxels);
	~World();

	Ref<Chunk> CreateChunk(Vector3 position, IVector3 extents);
	void BeginEdit(Ref<Chunk> chunk);
	void EndEdit(Ref<Chunk> chunk);

	void LoadBuffers();

private:
	std::vector<Ref<Chunk>> m_Chunks;

	Ref<StorageBuffer> m_VoxelStorage;
	Ref<StorageBuffer> m_ChunkStorage;
};