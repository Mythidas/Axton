#include "World.h"

#include <map>

World::World(uint32_t maxVoxels)
{
	StorageBuffer::Builder voxelBuilder;
	m_VoxelStorage = voxelBuilder.Size(sizeof(Voxel) * maxVoxels).Binding(4).DebugName("VoxelStorage").Build();

	Ref<Texture3D> tex = Texture3D::Create({ 1024, 1024, 1024 });
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

void World::LoadBuffers(const Camera& camera)
{
	std::map<float, Chunk::Buffer> chunks;
	for (size_t i = 0; i < m_Chunks.size(); i++)
	{
		float dist = glm::distance(camera.GetPosition(), m_Chunks[i]->GetPosition());
		chunks.emplace(dist, m_Chunks[i]->GetBuffer(dist));
	}

	std::vector<Chunk::Buffer> chunkBuffers;
	for (auto& cMap : chunks)
		chunkBuffers.push_back(cMap.second);
	
	StorageBuffer::Builder chunkBuilder;
	m_ChunkStorage = chunkBuilder.Size(sizeof(Chunk::Buffer) * chunkBuffers.size()).Usage(BufferUsage::STATIC_DRAW).Binding(2).DebugName("ChunkStorage").Build();
	m_ChunkStorage->SetData(chunkBuffers.data(), chunkBuffers.size() * sizeof(Chunk::Buffer));
}