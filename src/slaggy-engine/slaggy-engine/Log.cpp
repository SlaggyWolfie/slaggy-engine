#include "Log.hpp"

#include <fstream>

namespace slaggy
{
	std::string Log::output_path;

	std::vector<std::vector<std::string>> Log::serialize() const
	{
		std::vector<std::vector<std::string>> output;

		std::vector<std::string> row =
		{
			//"Test #", "Tree Type", "Maximum Depth", 
			"Frame", "Collision Tests", "Calculation Time"
		};

		std::string title = treeType + std::string(" Depth ") + std::to_string(maxDepth);
		if (!success) title += " (early end)";

		output.push_back({ { title } });
		output.push_back(row);

		for (const auto& snapshot : data)
		{
			row.clear();
			row.push_back(std::to_string(snapshot.frame));
			row.push_back(std::to_string(snapshot.collisionTests));
			row.push_back(std::to_string(snapshot.calculationTime));

			output.push_back(row);
		}

		return output;
	}

	void Log::output(std::vector<std::vector<std::string>> const& data, const std::string& path)
	{
		std::ofstream file(path);

		// CSV format
		const char separator = ',';

		// <empty>, <stat 1>, ... , <stat n>		

		for (unsigned i = 0; i < data.size(); ++i)
		{
			for (unsigned j = 0; j < data[i].size(); ++j)
			{
				file << data[i][j];
				if (j == data[i].size() - 1) file << std::endl;
				else file << separator;
			}
		}

		file.close();
	}

	void Log::output(const Log& log)
	{
		if (!output_path.empty())
			output(log.serialize(), output_path + log.treeType + std::to_string(log.maxDepth) + ".csv");
	}

	void Log::takeSnapshot()
	{
		data.push_back(snapshot);
	}
}
