#include "World.h"

#include <map>

World::World(uint32_t maxVoxels)
{
	StorageBuffer::Builder voxelBuilder;
	m_VoxelStorage = voxelBuilder.Size(sizeof(uint32_t) * maxVoxels).Binding(3).DebugName("VoxelStorage").Build();
}

Ref<OctreeChunk> World::CreateChunk(Vector3 position, IVector3 extents)
{
	uint32_t offset{ 0 };
	if (!m_Chunks.empty())
	{
		offset = m_Chunks.back()->Offset + Vector::Magnitude(m_Chunks.back()->Octree.Extents) / 4;
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

	static bool done = false;

	if (!done)
	{
		Timer timer("Voxels to GPU");

		uint32_t* voxelBuffer = static_cast<uint32_t*>(m_VoxelStorage->MapBufferPtr());
		for (size_t i = 0; i < m_Chunks.size(); i++)
		{
			float dist = glm::distance(camera.GetPosition(), m_Chunks[i]->Position);

			if (!done)
			{
				m_Chunks[i]->Offset = offset;
				uint32_t chunkOffset = 0;
				m_Chunks[i]->Octree.Root->SubmitToBuffer(voxelBuffer + offset, chunkOffset);
				offset += chunkOffset;
				MemTracker::SetSlot("VoxelStorage Used", offset * sizeof(uint32_t));
			}

			chunks.emplace(dist, m_Chunks[i]->CreateBuffer(dist, 0));
		}
		m_VoxelStorage->UnmapBufferPtr();
		done = true;
	}
	else
	{
		for (size_t i = 0; i < m_Chunks.size(); i++)
		{
			float dist = glm::distance(camera.GetPosition(), m_Chunks[i]->Position);
			chunks.emplace(dist, m_Chunks[i]->CreateBuffer(dist, 0));
		}
	}


	std::vector<OctreeChunk::Buffer> chunkBuffers;
	for (auto& cMap : chunks)
		chunkBuffers.push_back(cMap.second);
	
	StorageBuffer::Builder chunkBuilder;
	m_ChunkStorage = chunkBuilder.Size(sizeof(OctreeChunk::Buffer) * chunkBuffers.size()).Usage(BufferUsage::STATIC_DRAW).Binding(2).DebugName("ChunkStorage").Build();
	m_ChunkStorage->SetData(chunkBuffers.data(), chunkBuffers.size() * sizeof(OctreeChunk::Buffer));
}