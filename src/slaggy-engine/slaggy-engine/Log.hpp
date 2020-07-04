#pragma once
#ifndef LOG_HPP
#define LOG_HPP

#include <memory>
#include <vector>
#include <string>

namespace slaggy
{
	class Log
	{
	public:
		static std::string output_path;
		
		static Log& start();
		static Log& current();
		static Log end();

		static void output(std::vector<std::vector<std::string>> const& data, const std::string& path);
		static void output(const std::string& path);

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
		std::vector<std::vector<std::string>> serialize();

	private:
		static std::unique_ptr<Log> _current;
	};

	template <typename T>
	T Log::average(const std::vector<T>& values)
	{
		T average = 0;
		for (const auto& value : values) average += value;
		average /= values.size();

		return average;
	}
}
#endif