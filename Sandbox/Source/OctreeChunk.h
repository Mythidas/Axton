#pragma once

#include "Axton.h"
#include "Octree.h"

struct OctreeChunk
{
	struct Buffer
	{
		alignas(16) Vector3 MinExtent;
		alignas(16) Vector3 MaxExtent;
		alignas(16) IVector3 GridSize;
		float VoxelSize;
		float CameraDistance;
		uint32_t VoxelOffset;
		uint32_t MaterialIndex;
		uint32_t Albedos[255];
	};

	VoxelOctree Octree;
	Vector3 Position;
	uint32_t Offset{ 0 };
	uint32_t MaterialIndex{ 0 };
	uint32_t Albedos[255]{ 0xFFFFFFFF };

	OctreeChunk(Vector3 position, IVector3 extents)
		: Octree(VoxelOctree(extents)), Position(position)
	{
	}

	Buffer CreateBuffer(float distance, int lod)
	{
		float voxelSize = lod == 0 ? 1.0f : lod * 2.0f;

		OctreeChunk::Buffer buffer{};
		IVector3 extents = IVector3(Octree.GetHalfSize(lod));
		buffer.MinExtent = Position - Vector3(extents) * 0.5f * voxelSize;
		buffer.MaxExtent = Position + Vector3(extents) * 0.5f * voxelSize;
		buffer.GridSize = extents;
		buffer.VoxelSize = voxelSize;
		buffer.CameraDistance = distance;
		buffer.VoxelOffset = Offset;
		buffer.MaterialIndex = MaterialIndex;
		memcpy(&buffer.Albedos, &Albedos, sizeof(Albedos));
		return buffer;
	}
};