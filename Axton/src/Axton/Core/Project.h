#pragma once

#include "Axton/Utils/FileSystem.h"

namespace Axton
{
	class Project
	{
	public:
		struct Specs
		{
			FileSystem AssetsPath;

			Specs& setAssetsPath(const FileSystem& path) { AssetsPath = path; return *this; }
			void Build() { Construct(*this); }
		};

		static void Construct(const Specs& specs);

		static FileSystem& GetAssetsPath() { return s_Specs.AssetsPath; }

	private:
		static Specs s_Specs;
	};
}