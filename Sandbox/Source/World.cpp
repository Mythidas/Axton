#include "World.h"

#include <map>

World::World(uint32_t maxVoxels)
{
	m_VoxelStorage = RenderBuffer::Specs()
		.setBinding(3)
		.setSize(sizeof(uint32_t) * maxVoxels)
		.setUsage(BufferUsage::ShaderStorage)
		.setRate(BufferRate::Dyanamic)
		.setStages(BufferStage::Compute)
		.setStorage(BufferStorage::Local)
		.Build();

	m_MaterialStorage = RenderBuffer::Specs()
		.setBinding(4)
		.setSize(sizeof(Material) * 5)
		.setRate(BufferRate::Dyanamic)
		.setStages(BufferStage::Compute)
		.setStorage(BufferStorage::Local)
		.setUsage(BufferUsage::ShaderStorage)
		.Build();

	m_ChunkStorage = RenderBuffer::Specs()
		.setBinding(2)
		.setSize(sizeof(Chunk::Buffer) * 5)
		.setRate(BufferRate::PerFrame)
		.setStages(BufferStage::Compute)
		.setStorage(BufferStorage::Local)
		.setUsage(BufferUsage::ShaderStorage)
		.Build();
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

	std::vector<Material> materialBuffers;
	for (auto& mat : m_Materials)
		materialBuffers.push_back(*mat.get());

	m_MaterialStorage->SetData(materialBuffers.data(), m_Materials.size() * sizeof(Material), 0);

	return m_Materials.size() - 1;
}

void World::BeginEdit(Ref<Chunk> chunk)
{
	size_t size = Vector::Magnitude(chunk->Extents) / 4;
	uint32_t* voxelBuffer = static_cast<uint32_t*>(m_VoxelStorage->MapBufferPtr(chunk->Offset * sizeof(uint32_t), size * sizeof(uint32_t)));
	chunk->BeginEdit(voxelBuffer);
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
	
	m_ChunkStorage->SetData(chunkBuffers.data(), chunkBuffers.size() * sizeof(Chunk::Buffer), 0);
}