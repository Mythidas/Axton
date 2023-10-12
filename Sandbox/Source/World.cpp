#include "World.h"

World::World(uint32_t maxVoxels)
{
	StorageBuffer::Builder voxelBuilder;
	m_VoxelStorage = voxelBuilder.Size(sizeof(Voxel) * maxVoxels).Binding(4).DebugName("VoxelStorage").Build();
}

World::~World()
{

}

Ref<Chunk> World::CreateChunk(Vector3 position, IVector3 extents)
{
	uint32_t offset{ 0 };
	if (!m_Chunks.empty())
	{
		offset = m_Chunks.back()->m_Offset + m_Chunks.back()->GetFlatGridSize() / 4;
		while (offset % 4 != 0) offset++;
	}

	m_Chunks.push_back(CreateRef<Chunk>(position, extents, offset));

	MemTracker::SetSlot("VoxelStorage Used", (offset + m_Chunks.back()->GetFlatGridSize() / 4) * sizeof(Voxel));
	return m_Chunks.back();
}

void World::BeginEdit(Ref<Chunk> chunk)
{
	Voxel* voxelBuffer = static_cast<Voxel*>(m_VoxelStorage->MapBufferPtr());
	chunk->m_Voxels = voxelBuffer + chunk->m_Offset;
}

void World::EndEdit(Ref<Chunk> chunk)
{
	chunk->m_Voxels = nullptr;
	m_VoxelStorage->UnmapBufferPtr();
}

void World::LoadBuffers()
{
	std::vector<Chunk::Buffer> chunks;
	for (size_t i = 0; i < m_Chunks.size(); i++)
	{
		chunks.push_back(m_Chunks[i]->GetBuffer());
	}
	
	StorageBuffer::Builder chunkBuilder;
	m_ChunkStorage = chunkBuilder.Size(sizeof(Chunk::Buffer) * chunks.size()).Binding(2).DebugName("ChunkStorage").Build();
	m_ChunkStorage->SetData(chunks.data(), chunks.size() * sizeof(Chunk::Buffer));
}