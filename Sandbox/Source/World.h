#pragma once

#include "Axton.h"
#include "Chunk.h"
#include "OctreeChunk.h"

using namespace Axton;

class World
{
public:
	World(uint32_t maxVoxels);

	Ref<OctreeChunk> CreateChunk(Vector3 position, IVector3 extents);
	void BeginEdit(Ref<Chunk> chunk);
	void EndEdit(Ref<Chunk> chunk);

	void LoadBuffers(const Camera& camera);

	std::vector<Ref<OctreeChunk>>::iterator begin() { return m_Chunks.begin(); }
	std::vector<Ref<OctreeChunk>>::iterator end() { return m_Chunks.end(); }

private:
	std::vector<Ref<OctreeChunk>> m_Chunks;

	Ref<StorageBuffer> m_VoxelStorage;
	Ref<StorageBuffer> m_ChunkStorage;
};