#pragma once

#include "Axton.h"

using namespace Axton;

struct Voxel
{
	uint32_t MaterialIndex;
};

struct Chunk
{
	friend class World;
	const float VOXEL_SIZE = 1.0f;

	struct Buffer
	{
		alignas(16) Vector3 MinExtent;
		alignas(16) Vector3 MaxExtent;
		alignas(16) IVector3 GridSize;
		uint32_t VoxelOffset;
		uint32_t MaterialLookup[64];
	};

	Chunk(Vector3 position, IVector3 extents, uint32_t offset)
		: m_Position(position), m_Extents(extents), m_Offset(offset), m_Voxels(nullptr)
	{
		IVector3 buffs = extents % 4;
		m_Extents += buffs;
	}

	Chunk::Buffer GetBuffer() const
	{
		Chunk::Buffer buffer{};
		buffer.MinExtent = m_Position - Vector3(m_Extents) * 0.5f * VOXEL_SIZE;
		buffer.MaxExtent = m_Position + Vector3(m_Extents) * 0.5f * VOXEL_SIZE;
		buffer.VoxelOffset = m_Offset;
		buffer.GridSize = m_Extents;
		buffer.MaterialLookup[1] = 0;
		buffer.MaterialLookup[2] = 1;
		return buffer;
	}

	Vector3 GetPosition() { return m_Position; }
	IVector3 GetGridSize() { return m_Extents; }
	int GetFlatGridSize() { return (m_Extents.x * m_Extents.y * m_Extents.z) / 4; }
	//Voxel GetVoxel(UVector3 index) { return m_VoxelLocalData[CollapseIndex(index)]; }
	//Voxel GetVoxel(uint32_t index) { return m_VoxelLocalData[index]; }

	void SetPosition(Vector3 position) { m_Position = position; }
	void SetVoxel(UVector3 index, Voxel voxel) { PackVoxel(CollapseIndex(index), voxel); }
	void SetVoxel(uint32_t index, Voxel voxel) { PackVoxel(index, voxel); }

private:
	Vector3 m_Position;
	IVector3 m_Extents;
	uint32_t m_Offset;
	Voxel* m_Voxels;

private:
	UVector3 ExpandIndex(uint32_t index)
	{
		UVector3 indexParsed{ 0 };
		indexParsed.x = static_cast<uint32_t>(glm::floor(index % m_Extents.x));
		indexParsed.y = static_cast<uint32_t>(glm::floor((index / m_Extents.x) % m_Extents.y));
		indexParsed.z = static_cast<uint32_t>(glm::floor(index / (m_Extents.x * m_Extents.y)));
		return indexParsed;
	}

	uint32_t CollapseIndex(UVector3 index)
	{
		return (m_Extents.x * m_Extents.y * index.z) + (m_Extents.x * index.y) + index.x;
	}

	void PackVoxel(uint32_t index, Voxel voxel)
	{
		uint32_t buff = index % 4;
		uint32_t smallIndex = index / 4;

		switch (buff)
		{
		case 3: m_Voxels[smallIndex].MaterialIndex |= ((uint32_t)voxel.MaterialIndex << 24); break;
		case 2: m_Voxels[smallIndex].MaterialIndex |= ((uint32_t)voxel.MaterialIndex << 16); break;
		case 1: m_Voxels[smallIndex].MaterialIndex |= ((uint32_t)voxel.MaterialIndex << 8); break;
		case 0: m_Voxels[smallIndex].MaterialIndex |= ((uint32_t)voxel.MaterialIndex); break;
		}
	}
};