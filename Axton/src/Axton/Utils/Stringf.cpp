#include "axpch.h"
#include "Stringf.h"

namespace Axton
{
	std::vector<std::string> Stringf::Split(const std::string& source, const char delim)
	{
		std::vector<std::string> lines;
		std::string current = "";

		for (size_t i = 0; i < source.size(); i++)
		{
			if (source[i] == delim)
			{
				if (!current.empty())
				{
					lines.push_back(current);
					current = "";
				}

				continue;
			}

			current += source[i];
		}

		if (!current.empty())
			lines.push_back(current);

		return lines;
	}

	std::vector<std::string> Stringf::EachLine(const std::string& source)
	{
		return Split(source, '\n');
	}
}