#include "World.h"

#include <map>

World::World(uint32_t maxVoxels)
{
	StorageBuffer::Builder voxelBuilder;
	m_VoxelStorage = voxelBuilder.Size(sizeof(uint32_t) * maxVoxels / 4).Usage(BufferUsage::DYNAM_DRAW).Binding(3).DebugName("VoxelStorage").Build();
}

Ref<Chunk> World::CreateChunk(Vector3 position, IVector3 extents, bool sparse)
{
	if (sparse)
		m_Chunks.push_back(CreateRef<SparseChunk>(position, extents));
	else
	{
		uint32_t offset = 0;
		if (!m_Chunks.empty())
		{
			offset = m_Chunks.back()->Offset + Vector::Magnitude(m_Chunks.back()->Extents) / 4;
		}

		m_Chunks.push_back(CreateRef<DenseChunk>(position, extents));
		m_Chunks.back()->Offset = offset;

		MemTracker::SetSlot("Used VoxelStorage", offset);
	}

	return m_Chunks.back();
}

size_t World::AddMaterial(Ref<Material> material)
{
	m_Materials.push_back(material);
	return m_Materials.size() - 1;
}

void World::BeginEdit(Ref<Chunk> chunk)
{
	uint32_t* voxelBuffer = static_cast<uint32_t*>(m_VoxelStorage->MapBufferPtr());
	chunk->BeginEdit(voxelBuffer + chunk->Offset);
}

void World::EndEdit(Ref<Chunk> chunk)
{
	chunk->EndEdit();
	m_VoxelStorage->UnmapBufferPtr();
}

void World::LoadBuffers(const Camera& camera)
{
	std::map<float, Chunk::Buffer> chunks;

	for (size_t i = 0; i < m_Chunks.size(); i++)
	{
		float dist = glm::distance(camera.GetPosition(), m_Chunks[i]->Position);
		chunks.emplace(dist, m_Chunks[i]->CreateBuffer(dist));
	}

	std::vector<Chunk::Buffer> chunkBuffers;
	for (auto& cMap : chunks)
		chunkBuffers.push_back(cMap.second);
	
	StorageBuffer::Builder chunkBuilder;
	m_ChunkStorage = chunkBuilder.Size(sizeof(Chunk::Buffer) * chunkBuffers.size()).Usage(BufferUsage::STATIC_DRAW).Binding(2).DebugName("ChunkStorage").Build();
	m_ChunkStorage->SetData(chunkBuffers.data(), chunkBuffers.size() * sizeof(Chunk::Buffer));

	std::vector<Material> materialBuffers;
	for (auto& mat : m_Materials)
		materialBuffers.push_back(*mat.get());

	StorageBuffer::Builder materialBuilder;
	m_MaterialStorage = materialBuilder.Size(sizeof(Material) * materialBuffers.size()).Usage(BufferUsage::STATIC_DRAW).Binding(4).Build();
	m_MaterialStorage->SetData(materialBuffers.data(), m_Materials.size() * sizeof(Material));
}