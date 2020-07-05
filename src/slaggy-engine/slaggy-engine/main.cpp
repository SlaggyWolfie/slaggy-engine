#include "programs/LoggingProgram.hpp"
#include "programs/LoggingNoRenderingProgram.hpp"

#include <filesystem>
#include "Log.hpp"

int main(const int argc, char* argv[])
{	
	if (argc == 2)
	{
		int index = 0;
		if (strncmp(argv[index], "slaggy-engine", 13) == 0) index = 1;
		const std::string potentialPath(argv[index]);

		if (!std::filesystem::is_directory(potentialPath)) return 1;

		slaggy::Log::output_path = potentialPath + "/";
	}
	
	//return slaggy::LoggingNoRenderingProgram().run();
	return slaggy::LoggingProgram().run();
}
