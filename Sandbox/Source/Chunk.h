#pragma once

#include "Axton.h"
#include "Octree.h"

const float STANDARD_VOXEL_SIZE = 1.0f;

class Chunk
{
public:
	struct Buffer
	{
		alignas(16) Vector3 MinExtent;
		alignas(16) Vector3 MaxExtent;
		alignas(16) IVector3 GridSize;
		bool Sparse;
		float VoxelSize;
		float CameraDistance;
		uint32_t VoxelOffset;
		uint32_t MaterialIndex;
		uint32_t Albedos[255];
	};

public:
	virtual void BeginEdit(uint32_t* buffer) = 0;
	virtual void EndEdit() = 0;

	virtual void SubmitToBuffer(uint32_t* buffer, uint32_t& offset) = 0;

	virtual void SetVoxel(int index, uint8_t voxel) = 0;
	virtual void SetVoxel(IVector3 index, uint8_t voxel) = 0;

	virtual Chunk::Buffer CreateBuffer(float distance) = 0;

	Vector3 Position{ 0 };
	IVector3 Extents{ 0 };
	uint32_t Offset{ 0 };
	uint32_t MaterialIndex{ 0 };
	uint32_t Albedos[255]{ 0xFFFFFFFF };

	IVector3 ExpandIndex(int index)
	{
		IVector3 parse{ 0 };
		parse.x = static_cast<int>(floor(index % Extents.x));
		parse.y = static_cast<int>(floor((index / Extents.x) % Extents.y));
		parse.z = static_cast<int>(floor(index / (Extents.x * Extents.y)));
		return parse;
	}

	int CollapseIndex(IVector3 index)
	{
		return (Extents.x * Extents.y * index.z) + (Extents.x * index.y) + index.x;
	}
};

class SparseChunk : public Chunk
{
public:
	SparseChunk(Vector3 position, IVector3 extents)
		: Octree(VoxelOctree(extents))
	{
		Position = position;
		Extents = extents;
	}

	virtual void BeginEdit(uint32_t* buffer) override
	{
		// Stores data in Octree class
	}

	virtual void EndEdit() override
	{
		Octree.Root->Refresh();
	}

	virtual void SubmitToBuffer(uint32_t* buffer, uint32_t& offset) override
	{
		Octree.Root->SubmitToBuffer(buffer, offset);
	}

	virtual void SetVoxel(int index, uint8_t voxel) override
	{
		Octree.SetVoxelOriginal(ExpandIndex(index), voxel);
	}

	virtual void SetVoxel(IVector3 index, uint8_t voxel) override
	{
		Octree.SetVoxelOriginal(index, voxel);
	}

	virtual Chunk::Buffer CreateBuffer(float distance) override
	{
		int lod = 0;
		float voxelSize = lod == 0 ? 1.0f : lod * 2.0f;

		Chunk::Buffer buffer{};
		IVector3 extents = IVector3(Octree.GetHalfSize(lod));
		buffer.MinExtent = Position - Vector3(extents) * 0.5f * voxelSize;
		buffer.MaxExtent = Position + Vector3(extents) * 0.5f * voxelSize;
		buffer.GridSize = extents;
		buffer.Sparse = true;
		buffer.VoxelSize = voxelSize;
		buffer.CameraDistance = distance;
		buffer.VoxelOffset = Offset;
		buffer.MaterialIndex = MaterialIndex;
		memcpy(&buffer.Albedos, &Albedos, sizeof(Albedos));
		return buffer;
	}

	VoxelOctree Octree;
};

class DenseChunk : public Chunk
{
public:
	DenseChunk(Vector3 position, IVector3 extents)
	{
		Position = position;

		IVector3 pad = extents % 4;
		Extents = extents + (4 - pad) % 4;
	}

	virtual void BeginEdit(uint32_t* buffer) override
	{
		Voxels = buffer;
	}

	virtual void EndEdit() override
	{
		Voxels = nullptr;
	}

	virtual void SubmitToBuffer(uint32_t* buffer, uint32_t& offset) override
	{
		offset += Offset;
	}

	virtual void SetVoxel(int index, uint8_t voxel) override
	{
		PackVoxel(index, voxel);
	}

	virtual void SetVoxel(IVector3 index, uint8_t voxel) override
	{
		PackVoxel(CollapseIndex(index), voxel);
	}

	virtual Chunk::Buffer CreateBuffer(float distance) override
	{
		Chunk::Buffer buffer{};
		buffer.MinExtent = Position - Vector3(Extents) * 0.5f * STANDARD_VOXEL_SIZE;
		buffer.MaxExtent = Position + Vector3(Extents) * 0.5f * STANDARD_VOXEL_SIZE;
		buffer.GridSize = Extents;
		buffer.Sparse = false;
		buffer.VoxelSize = STANDARD_VOXEL_SIZE;
		buffer.CameraDistance = distance;
		buffer.VoxelOffset = Offset;
		buffer.MaterialIndex = MaterialIndex;
		memcpy(&buffer.Albedos, &Albedos, sizeof(Albedos));
		return buffer;
	}

	uint32_t* Voxels{ nullptr };

private:
	void PackVoxel(int index, uint8_t voxel)
	{
		if (Voxels == nullptr)
		{
			AX_ASSERT(false, "Failed to map buffer before editing Voxel Grid!");
			return;
		}

		int bit = index % 4;
		int smallIndex = glm::floor(index / 4.0f);

		Voxels[smallIndex] = Bit::SetTo(Voxels[smallIndex], voxel, bit * 8);
	}
};