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
		static Log& start();
		static Log& current();
		static void end();
		
		static std::vector<std::vector<std::string>> serialize();
		static void output(std::vector<std::vector<std::string>> const& data, const std::string& path);
		static void output(const std::string& path);
		
		unsigned testNumber;
		std::string treeType = "";
		unsigned maxDepth = -1;

		std::vector<unsigned> collisionTests{ };
		std::vector<double> timeCalcCollTests{ };

	private:
		static std::unique_ptr<Log> _current;		
		static std::vector<Log> _results;

		template <typename T>
		static T average(const std::vector<T>& values);
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