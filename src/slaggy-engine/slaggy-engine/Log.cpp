#include "Log.hpp"

#include <fstream>

namespace slaggy
{
	std::vector<Log> Log::_results = { };
	std::unique_ptr<Log> Log::_current = nullptr;

	Log& Log::start()
	{
		_current = std::make_unique<Log>();
		return *_current;
	}

	void Log::end()
	{
		_results.push_back(*_current);
		_current = nullptr;
	}

	Log& Log::current()
	{
		return *_current;
	}

	std::vector<std::vector<std::string>> Log::serialize()
	{
		std::vector<std::vector<std::string>> output;

		std::vector<std::string> row =
		{
			"Test #", "Tree Type", "Maximum Depth", "Collision Tests",
			"Calculation Time for Collisions Tests"
		};

		output.push_back(row);

		for (const auto& log : _results)
		{
			row.clear();
			row.push_back(std::to_string(log.testNumber));
			row.push_back(log.treeType);
			row.push_back(std::to_string(log.maxDepth));
			row.push_back(std::to_string(average<unsigned>(log.collisionTests)));
			row.push_back(std::to_string(average<double>(log.timeCalcCollTests)));

			output.push_back(row);
		}

		return output;
	}

	void Log::output(std::vector<std::vector<std::string>> const& data, const std::string& path)
	{
		std::ofstream file(path);

		const char separator = ',';

		// <empty>, <stat 1>, ... , <stat n>		
		for (const auto& i : data)
		{
			for (const auto& j : i)
			{
				file << j;

				if (j == i.back())
					file << separator;
				else file << std::endl;
			}
		}

		file.close();
	}

	void Log::output(const std::string& path)
	{
		output(serialize(), path);
	}
}
