#include "World.h"

#include <map>

World::World(uint32_t maxVoxels)
{
	StorageBuffer::Builder voxelBuilder;
	m_VoxelStorage = voxelBuilder.Size(sizeof(uint32_t) * maxVoxels).Binding(4).DebugName("VoxelStorage").Build();
}

Ref<OctreeChunk> World::CreateChunk(Vector3 position, IVector3 extents)
{
	uint32_t offset{ 0 };
	if (!m_Chunks.empty())
	{
		offset = m_Chunks.back()->Offset + Vector::Magnitude(m_Chunks.back()->VoxelOctree.Extents) / 4;
		while (offset % 4 != 0) offset++;
	}

	m_Chunks.push_back(CreateRef<OctreeChunk>(position, extents, offset));
	return m_Chunks.back();
}

void World::BeginEdit(Ref<Chunk> chunk)
{
	uint32_t* voxelBuffer = static_cast<uint32_t*>(m_VoxelStorage->MapBufferPtr());
	//chunk->m_Voxels = voxelBuffer + chunk->m_Offset;
}

void World::EndEdit(Ref<Chunk> chunk)
{
	chunk->m_Voxels = nullptr;
	m_VoxelStorage->UnmapBufferPtr();
}

void World::LoadBuffers(const Camera& camera)
{
	std::map<float, OctreeChunk::Buffer> chunks;
	uint32_t offset = 0;

	uint32_t* voxelBuffer = static_cast<uint32_t*>(m_VoxelStorage->MapBufferPtr());
	for (size_t i = 0; i < m_Chunks.size(); i++)
	{
		float dist = glm::distance(camera.GetPosition(), m_Chunks[i]->Position);

		int lod = Mathf::Min(int(floor(dist / 800.0f)), m_Chunks[i]->VoxelOctree.Levels - 3);
		int divisor = lod == 0 ? 1 : 8 * lod;

		if (m_Chunks[i]->LastLOD != lod)
		{
			m_Chunks[i]->Offset = offset;
			for (int j = 0; j < Vector::Magnitude(m_Chunks[i]->VoxelOctree.Extents) / divisor; j += 4)
			{
				uint8_t v1 = m_Chunks[i]->VoxelOctree.GetVoxelRaw(j, lod);
				uint8_t v2 = m_Chunks[i]->VoxelOctree.GetVoxelRaw(j + 1, lod);
				uint8_t v3 = m_Chunks[i]->VoxelOctree.GetVoxelRaw(j + 2, lod);
				uint8_t v4 = m_Chunks[i]->VoxelOctree.GetVoxelRaw(j + 3, lod);
				int test = j / (4) + m_Chunks[i]->Offset;
				voxelBuffer[j / (4) + m_Chunks[i]->Offset] = Bit::U32_4x8(v1, v2, v3, v4);
			}
		}

		offset += Vector::Magnitude(m_Chunks[i]->VoxelOctree.Extents) / 4;
		chunks.emplace(dist, m_Chunks[i]->CreateBuffer(dist, lod));
	}
	m_VoxelStorage->UnmapBufferPtr();

	std::vector<OctreeChunk::Buffer> chunkBuffers;
	for (auto& cMap : chunks)
		chunkBuffers.push_back(cMap.second);
	
	StorageBuffer::Builder chunkBuilder;
	m_ChunkStorage = chunkBuilder.Size(sizeof(OctreeChunk::Buffer) * chunkBuffers.size()).Usage(BufferUsage::STATIC_DRAW).Binding(2).DebugName("ChunkStorage").Build();
	m_ChunkStorage->SetData(chunkBuffers.data(), chunkBuffers.size() * sizeof(OctreeChunk::Buffer));
}