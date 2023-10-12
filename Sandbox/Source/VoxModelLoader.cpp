#include "VoxModelLoader.h"

VoxModelLoader::VoxModelLoader(const std::string& path)
	: m_Path(path)
{
}

Ref<Chunk> VoxModelLoader::GenChunk(World& world, Vector3 position)
{
	FileSystem file(m_Path);
	std::vector<uint8_t> buffer = file.ToBuffer();

	size_t currentIndex = 0;

	uint32_t vox = Bit::U32_4x8('V', 'O', 'X', ' ');
	FindValueIndex(buffer, vox, currentIndex);
	uint32_t version = GetNextValue(buffer, currentIndex);

	if (version != 200 && version != 150)
	{
		Log::Warn(".Vox file incorrect version [{0}]! Model not loaded.", version);
		return nullptr;
	}

	uint32_t size = Bit::U32_4x8('S', 'I', 'Z', 'E');
	FindValueIndex(buffer, size, currentIndex);
	uint32_t totalSize = GetNextValue(buffer, currentIndex);
	uint32_t childSize = GetNextValue(buffer, currentIndex);
	IVector3 extents = { GetNextValue(buffer, currentIndex) , GetNextValue(buffer, currentIndex) , GetNextValue(buffer, currentIndex) };

	uint32_t xyzi = Bit::U32_4x8('X', 'Y', 'Z', 'I');
	FindValueIndex(buffer, xyzi, currentIndex);
	uint32_t xyziSize = GetNextValue(buffer, currentIndex);
	uint32_t xyziChildSize = GetNextValue(buffer, currentIndex);
	uint32_t xyziCount = GetNextValue(buffer, currentIndex);

	Ref<Chunk> newChunk = world.CreateChunk(position, { extents.x, extents.z, extents.y });

	world.BeginEdit(newChunk);
	for (int i = 0; i < xyziCount; i++)
	{
		IVector4 voxelVal = Vector::ConvertIV4(GetNextValue(buffer, currentIndex));
		uint32_t voxelValIndex = newChunk->CollapseIndex({voxelVal.x, voxelVal.z, voxelVal.y});
		newChunk->SetVoxel(voxelValIndex, { (uint32_t)voxelVal.w });
	}

	uint32_t rgba = Bit::U32_4x8('R', 'G', 'B', 'A');
	FindValueIndex(buffer, rgba, currentIndex);
	uint32_t rgba0 = GetNextValue(buffer, currentIndex);

	for (int i = 0; i < 255; i++)
	{
		newChunk->SetMaterial(i, { GetNextValue(buffer, currentIndex) });
	}
	world.EndEdit(newChunk);

	return newChunk;
}

void VoxModelLoader::FindValueIndex(const std::vector<uint8_t>& buffer, uint32_t value, size_t& currentIndex)
{
	if (currentIndex > buffer.size() - 4) return;

	for (size_t i = currentIndex; i < buffer.size() - 4; i += 4)
	{
		uint32_t stepValue = Bit::U32_4x8(buffer[i], buffer[i + 1], buffer[i + 2], buffer[i + 3]);
		if (stepValue == value)
		{
			currentIndex = i + 4;
			break;
		}
	}
}

uint32_t VoxModelLoader::GetNextValue(const std::vector<uint8_t>& buffer, size_t& currentIndex)
{
	if (currentIndex > buffer.size() - 4)
	{
		currentIndex += 4;
		return 0;
	}

	uint32_t value = Bit::U32_4x8(buffer[currentIndex], buffer[currentIndex + 1], 
		buffer[currentIndex + 2], buffer[currentIndex + 3]);
	currentIndex += 4;
	return value;
}