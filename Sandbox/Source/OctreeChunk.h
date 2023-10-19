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
		uint32_t Materials[255];
	};

	VoxelOctree Octree;
	Vector3 Position;
	uint32_t Offset;
	uint32_t Materials[255]{ 0xFFFFFFFF };
	int LastLOD = -1;

	OctreeChunk(Vector3 position, IVector3 extents, uint32_t offset)
		: Octree(VoxelOctree(extents)), Position(position), Offset(offset)
	{
	}

	Buffer CreateBuffer(float distance, int lod)
	{
		LastLOD = lod;
		float voxelSize = lod == 0 ? 1.0f : lod * 2.0f;

		OctreeChunk::Buffer buffer{};
		IVector3 extents = IVector3(Octree.GetHalfSize(lod));
		buffer.MinExtent = Position - Vector3(extents) * 0.5f * voxelSize;
		buffer.MaxExtent = Position + Vector3(extents) * 0.5f * voxelSize;
		buffer.GridSize = extents;
		buffer.VoxelSize = voxelSize;
		buffer.CameraDistance = distance;
		buffer.VoxelOffset = Offset;
		memcpy(&buffer.Materials, &Materials, sizeof(Materials));
		return buffer;
	}
};