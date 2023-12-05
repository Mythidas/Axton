#include "axpch.h"
#include "Project.h"

namespace Axton
{
	Project::Specs Project::s_Specs;

	void Project::Construct(const Specs& specs)
	{
		s_Specs = specs;

		if (specs.AssetsPath.ToString().empty())
		{
			s_Specs.AssetsPath = FileSystem(FS::current_path().parent_path().parent_path().string() + "/Assets");
		}
	}
}