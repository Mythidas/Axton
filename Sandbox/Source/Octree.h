//#pragma once
//
//#include "Axton.h"
//
//#include <map>
//
//using namespace Axton;
//
//struct Node
//{
//	std::array<Node*, 8> Children{ nullptr };
//	uint8_t Material{ 0 };
//
//	void Refresh()
//	{
//		if (IsLeaf()) return;
//
//		uint8_t childCount = 0;
//		for (int i = 0; i < 8; i++)
//		{
//			if (Children[i])
//			{
//				Children[i]->Refresh();
//				if (Children[i]->IsLeaf())
//					childCount++;
//			}
//		}
//
//		if (childCount != 8) return;
//
//		uint8_t mat = Children[0]->Material;
//		for (int i = 1; i < 8; i++)
//		{
//			if (Children[i]->Material != mat)
//			{
//				UpdateMaterial();
//				return;
//			}
//		}
//
//		Material = mat;
//		for (int i = 0; i < 8; i++)
//		{
//			delete Children[i];
//			Children[i] = nullptr;
//		}
//	}
//
//	void UpdateMaterial()
//	{
//		if (IsLeaf()) return;
//
//		std::map<uint8_t, uint8_t> mats;
//		for (int i = 0; i < 8; i++)
//		{
//			if (Children[i])
//			{
//				if (mats.find(Children[i]->Material) != mats.end())
//				{
//					mats[Children[i]->Material]++;
//				}
//				else
//				{
//					mats[Children[i]->Material] = 1;
//				}
//			}
//		}
//
//		Material = mats[0];
//	}
//
//	void SubmitToBuffer(uint32_t* buffer, uint32_t& offset, bool isRoot = true)
//	{
//		if (!isRoot && IsLeaf())
//			return;
//
//		uint32_t currentOffset = offset;
//		offset += 8;
//
//		for (int i = 0; i < 8; i++)
//		{
//			if (Children[i])
//			{
//				if (Children[i]->IsLeaf())
//				{
//					buffer[currentOffset + i] = Bit::U32_4x8(Children[i]->Material, 0, 0, 1);
//				}
//				else
//				{
//					uint32_t offsetDiff = offset - currentOffset;
//
//					uint8_t offsetX = offsetDiff % 255;
//					uint8_t offsetY = ((offsetDiff / 255) % 255);
//					uint8_t offsetZ = offsetDiff / (255 * 255);
//
//					buffer[currentOffset + i] = Bit::U32_4x8(offsetX, offsetY, offsetZ, 0);
//					Children[i]->SubmitToBuffer(buffer, offset, false);
//				}
//			}
//			else
//			{
//				if (isRoot)
//					buffer[currentOffset + i] = Bit::U32_4x8(Material, 0, 0, 1);
//				else
//					buffer[currentOffset + i] = Bit::U32_4x8(0, 0, 0, 1);
//			}
//		}
//	}
//
//	bool IsLeaf()
//	{
//		for (int i = 0; i < 8; i++)
//		{
//			if (Children[i])
//				return false;
//		}
//
//		return true;
//	}
//
//	size_t Size()
//	{
//		if (IsLeaf()) return 1;
//
//		size_t size = 0;
//		for (int i = 0; i < 8; i++)
//		{
//			if (Children[i])
//				size += Children[i]->Size();
//		}
//
//		return size;
//	}
//};
//
//class VoxelOctree
//{
//public:
//	IVector3 Extents;
//	IVector3 OriginalExtents;
//	Node* Root;
//	int Levels;
//
//	VoxelOctree(IVector3 extents)
//		: OriginalExtents(extents)
//	{
//		int size = Mathf::Max(Mathf::Max(extents.x, extents.y), extents.z);
//		Extents = IVector3(pow(2, ceil(log2(size))));
//		Root = new Node();
//		Levels = log2(Extents.x);
//	}
//
//	void SetVoxel(IVector3 index, uint8_t voxel)
//	{
//		Node* node = SetNode(index);
//		node->Material = voxel;
//	}
//
//	void SetVoxelOriginal(IVector3 index, uint8_t voxel)
//	{
//		IVector3 offset = Vector::Clamp((Extents - OriginalExtents), IVector3(0), OriginalExtents);
//		offset.x = offset.x >> 1;
//		offset.z = offset.z >> 1;
//		offset.y = offset.y >> 1;
//		Node* node = SetNode(index + offset);
//		node->Material = voxel;
//	}
//
//	uint8_t GetVoxel(IVector3 index)
//	{
//		Node* node = GetNode(index);
//		return !node ? 0 : node->Material;
//	}
//
//	uint8_t GetVoxel(int index)
//	{
//		return GetVoxel(ExpandIndex(index));
//	}
//
//	int GetHalfSize(int lod)
//	{
//		return 1 << (Levels - lod);
//	}
//
//private:
//	Node* SetNode(IVector3 index)
//	{
//		Node* current = Root;
//		IVector3 tIndex = index;
//		for (int i = 0; i < Levels; i++)
//		{
//			int childIndex = 0;
//			int halfSize = GetHalfSize(i + 1);
//
//			if (tIndex.x >= halfSize)
//			{
//				childIndex |= 4;
//				tIndex.x -= halfSize;
//			}
//
//			if (tIndex.y >= halfSize)
//			{
//				childIndex |= 2;
//				tIndex.y -= halfSize;
//			}
//
//			if (tIndex.z >= halfSize)
//			{
//				childIndex |= 1;
//				tIndex.z -= halfSize;
//			}
//
//			if (!current->Children[childIndex])
//				current->Children[childIndex] = new Node();
//
//			current = current->Children[childIndex];
//		}
//
//		return current;
//	}
//
//	Node* GetNode(IVector3 index)
//	{
//		Node* current = Root;
//
//		for (int i = 0; i < Levels; i++)
//		{
//			int childIndex = 0;
//			int halfSize = GetHalfSize(i + 1);
//
//			if (index.x >= halfSize)
//			{
//				childIndex |= 4;
//				index.x -= halfSize;
//			}
//
//			if (index.y >= halfSize)
//			{
//				childIndex |= 2;
//				index.y -= halfSize;
//			}
//
//			if (index.z >= halfSize)
//			{
//				childIndex |= 1;
//				index.z -= halfSize;
//			}
//
//			if (!current->Children[childIndex])
//				return nullptr;
//
//			current = current->Children[childIndex];
//
//			if (current->IsLeaf())
//				return current;
//		}
//
//		return current;
//	}
//
//	IVector3 ExpandIndex(int index)
//	{
//		IVector3 parsed{ 0 };
//		parsed.x = int(floor(index % Extents.x));
//		parsed.y = int(floor((index / Extents.x) % Extents.y));
//		parsed.z = int(floor(index / (Extents.x * Extents.y)));
//		return parsed;
//	}
//};