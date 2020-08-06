#pragma once
#ifndef LOG_HPP
#define LOG_HPP

#include <memory>
#include <vector>
#include <string>
#include <algorithm>

namespace slaggy
{
	class Log
	{
	public:
		static std::string output_path;

		static void output(std::vector<std::vector<std::string>> const& data, const std::string& path);
		static void output(const Log& log);

		template <typename T>
		static T average(const std::vector<T>& values);

		bool success = true;

		std::string treeType = "";
		unsigned maxDepth = -1;

		struct Snapshot
		{
			unsigned collisionTests = -1;
			double calculationTime = -1;
			unsigned frame = -1;
		} snapshot;

		std::vector<Snapshot> data{ };

		void takeSnapshot();
		[[nodiscard]] std::vector<std::vector<std::string>> serialize() const;
	};

	template <typename T>
	T Log::average(const std::vector<T>& values)
	{
		T average = 0;
		for (const auto& value : values) average += value;
		average /= values.size();

		return average;
	}

	template<>
	inline Log::Snapshot Log::average(const std::vector<Log::Snapshot>& values)
	{
		Snapshot average;
		for (const auto& snapshot : values)
		{
			average.frame = std::max(average.frame, snapshot.frame);
			average.calculationTime = snapshot.calculationTime;
			average.collisionTests = snapshot.collisionTests;
		}

		average.calculationTime /= values.size();
		average.collisionTests /= values.size();

		return average;
	}

	template<>
	inline Log Log::average(const std::vector<Log>& values)
	{
		Log average = values[0];
		std::vector<std::vector<Log::Snapshot>> snapshots;
		
		// TODO
		for (unsigned i = 0; i < values.size(); ++i)
		{
			for (unsigned j = 0; j < values[i].data.size(); ++j)
			{
				//snapshots.
			}
		}

		return average;
	}	
}
#endif