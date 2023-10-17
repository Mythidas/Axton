#pragma once

#include "Axton.h"

using namespace Axton;

struct Voxel
{
	uint32_t MaterialIndex{ 0 };
};

struct Material
{
	uint32_t Albedo{ 0 };
};

struct Chunk
{
	friend class VoxModelLoader;
	friend class World;
	const float VOXEL_SIZE = 1.0f;

	struct Buffer
	{
		alignas(16) Vector3 MinExtent;
		alignas(16) Vector3 MaxExtent;
		alignas(16) IVector3 GridSize;
		float CameraDistance;
		uint32_t VoxelOffset;
		Material Materials[255];
	};

	Chunk(Vector3 position, IVector3 extents, uint32_t offset)
		: m_Position(position), m_Extents(extents), m_Offset(offset), m_Voxels(nullptr)
	{
		Material white{ Bit::U32_4x8(130, 130, 130, 255) };
		Material black{ Bit::U32_4x8(80, 130, 200, 255) };
		m_Materials[0] = white;
		m_Materials[1] = black;
	}

	Chunk::Buffer GetBuffer(float distance) const
	{
		Chunk::Buffer buffer{};
		buffer.MinExtent = m_Position - Vector3(m_Extents) * 0.5f * VOXEL_SIZE;
		buffer.MaxExtent = m_Position + Vector3(m_Extents) * 0.5f * VOXEL_SIZE;
		buffer.CameraDistance = distance;
		buffer.VoxelOffset = m_Offset;
		buffer.GridSize = m_Extents;
		memcpy(&buffer.Materials, &m_Materials, sizeof(m_Materials));
		return buffer;
	}

	Vector3 GetPosition() { return m_Position; }
	IVector3 GetGridSize() { return m_Extents; }
	int GetFlatGridSize() { return m_Extents.x * m_Extents.y * m_Extents.z; }
	//Voxel GetVoxel(UVector3 index) { return m_VoxelLocalData[CollapseIndex(index)]; }
	//Voxel GetVoxel(uint32_t index) { return m_VoxelLocalData[index]; }

	void SetPosition(Vector3 position) { m_Position = position; }
	void SetVoxel(UVector3 index, Voxel voxel) { PackVoxel(CollapseIndex(index) ,voxel.MaterialIndex); }
	void SetVoxel(uint32_t index, Voxel voxel) { m_Voxels[index] = voxel; }
	void SetMaterial(uint32_t index, Material mat) { m_Materials[index] = mat; }

private:
	Vector3 m_Position;
	IVector3 m_Extents;
	uint32_t m_Offset;
	Material m_Materials[255];
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

	void PackVoxel(uint32_t index, uint8_t voxel)
	{
		uint32_t buff = index % 4;
		uint32_t smallIndex = glm::floor(index / uint32_t(4));

		switch (buff)
		{
		case 3: m_Voxels[smallIndex].MaterialIndex = Bit::SetTo(m_Voxels[smallIndex].MaterialIndex, 
			(uint8_t)voxel, 24); break;
		case 2: m_Voxels[smallIndex].MaterialIndex = Bit::SetTo(m_Voxels[smallIndex].MaterialIndex, 
			(uint8_t)voxel, 16); break;
		case 1: m_Voxels[smallIndex].MaterialIndex = Bit::SetTo(m_Voxels[smallIndex].MaterialIndex, 
			(uint8_t)voxel, 8); break;
		case 0: m_Voxels[smallIndex].MaterialIndex = Bit::SetTo(m_Voxels[smallIndex].MaterialIndex, 
			(uint8_t)voxel, 0); break;
		}
	}

	void PackLooseVoxel(uint32_t index, uint8_t voxel)
	{
		m_Voxels[index].MaterialIndex = Bit::SetTo(m_Voxels[index].MaterialIndex, voxel, 0);
		m_Voxels[index].MaterialIndex = Bit::SetTo(m_Voxels[index].MaterialIndex, voxel, 8);
		m_Voxels[index].MaterialIndex = Bit::SetTo(m_Voxels[index].MaterialIndex, voxel, 16);
		m_Voxels[index].MaterialIndex = Bit::SetTo(m_Voxels[index].MaterialIndex, voxel, 24);
	}
};