#include <filesystem>

#include "programs/LoggingProgram.hpp"
#include "programs/LoggingNoRenderingProgram.hpp"
#include "programs/SpatialTestingProgram.hpp"

#include "Log.hpp"
#include <iostream>

int main(const int argc, char* argv[])
{
	if (argc == 2)
	{
		const std::string potentialPath(argv[argc - 1]);

		if (!std::filesystem::is_directory(potentialPath)) return 1;

		slaggy::Log::output_path = potentialPath + "/";
		std::cout << "Found directory: " << slaggy::Log::output_path << std::endl;
	}

	//return slaggy::LoggingNoRenderingProgram().run();
	//return slaggy::LoggingProgram().run();
	return slaggy::SpatialTestingProgram().run();
}
