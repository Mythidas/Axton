#pragma once

#include "Axton.h"

using namespace Axton;

struct Node
{
	std::array<Node*, 8> Children{ 0 };
	uint8_t MaterialIndex{ 0 };
	bool IsStale{ true };

	void Rebuild()
	{
		if (!IsStale || IsLeaf()) return;
		IsStale = false;

		std::unordered_map<uint8_t, uint8_t> materials;
		for (int i = 0; i < 8; i++)
		{
			if (Children[i])
			{
				Children[i]->Rebuild();

				if (materials.find(Children[i]->MaterialIndex) != materials.end())
				{
					materials[Children[i]->MaterialIndex] = materials[Children[i]->MaterialIndex] + 1;
				}
				else
				{
					materials[Children[i]->MaterialIndex] = 1;
				}
			}
		}

		uint8_t largest = 0;
		for (auto& pair : materials)
		{
			if (pair.second > largest && pair.first != 0)
			{
				MaterialIndex = pair.first;
				largest = pair.second;
			}
		}
	}

	bool IsLeaf()
	{
		for (int i = 0; i < 8; i++)
		{
			if (Children[i])
				return false;
		}

		return true;
	}
};

struct Octree
{
	Node* Root;
	IVector3 Extents;
	IVector3 Padding;
	int Levels{ 0 };
	bool HasChanged{ true };

	Octree(IVector3 extents)
	{
		int largest = Mathf::Max(Mathf::Max(extents.x, extents.y), extents.z);
		Extents = IVector3(pow(2, ceil(log2(largest))));
		Padding = Extents - extents;
		Levels = log2(Extents.x);
		Root = new Node();
	}

	~Octree()
	{
	}

	void Rebuild()
	{
		Root->Rebuild();
	}

	void SetVoxel(IVector3 index, uint8_t material)
	{
		GetNode(index + Padding / 2)->MaterialIndex = material;
		HasChanged = true;
	}

	void SetVoxel(int index, uint8_t material)
	{
		SetVoxel(ExpandIndex(index), material);
		HasChanged = true;
	}

	uint8_t GetVoxelRaw(int index, int lod = 0)
	{
		Node* node = FindNode(ExpandIndex(index, lod), lod);
		return node ? node->MaterialIndex : 0;
	}

	IVector3 GetOriginalExtents()
	{
		return Extents - Padding;
	}

	int GetExtentSize(int lod) const
	{
		return 1 << (Levels - lod);
	}

private:
	Node* GetNode(IVector3 index)
	{
		Node* current = Root;
		Root->IsStale = true;

		for (int i = 0; i < Levels; i++)
		{
			int child = 0;
			int halfSize = 1 << (Levels - i - 1);

			if (index.x >= halfSize)
			{
				child |= 1;
				index.x -= halfSize;
			}
			if (index.y >= halfSize)
			{
				child |= 2;
				index.y -= halfSize;
			}
			if (index.z >= halfSize)
			{
				child |= 4;
				index.z -= halfSize;
			}

			if (!current->Children[child]) current->Children[child] = new Node();

			current = current->Children[child];
			current->IsStale = true;
		}

		return current;
	}

	Node* FindNode(IVector3 index, int lod)
	{
		Node* current = Root;
		int maxLevel = Mathf::Max(Levels - lod, 1);

		for (int i = 0; i < maxLevel; i++)
		{
			int child = 0;
			int halfSize = 1 << (Levels - i - 1);

			if (index.x >= halfSize)
			{
				child |= 1;
				index.x -= halfSize;
			}
			if (index.y >= halfSize)
			{
				child |= 2;
				index.y -= halfSize;
			}
			if (index.z >= halfSize)
			{
				child |= 4;
				index.z -= halfSize;
			}

			if (!current->Children[child]) 
				return nullptr;

			current = current->Children[child];
		}

		return current;
	}

	IVector3 ExpandIndex(int index, int lod = 0)
	{
		IVector3 parsed{ 0 };
		IVector3 extents = IVector3(GetExtentSize(lod));
		parsed.x = floor(index % extents.x);
		parsed.y = floor((index / extents.x) % extents.y);
		parsed.z = floor(index / (extents.x * extents.y));
		return lod == 0 ? parsed : parsed * 2 * lod;
	}
};