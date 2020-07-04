#include "programs/LoggingProgram.hpp"
#include "programs/LoggingNoRenderingProgram.hpp"

#include <filesystem>
#include "Log.hpp"
//#include <iostream>

int main(const int argc, char* argv[])
{
	//std::cout << "Params Count: " << argc << std::endl;

	//std::cout << "Params: \n";
	//for (auto i = 0; i < argc; ++i)
	//{
	//	std::cout << argv[i] << '\n';
	//}

	//std::cout << std::endl;
	
	if (argc == 2)
	{
		int index = 0;
		if (strncmp(argv[index], "slaggy-engine", 13) == 0) index = 1;
		const std::string potentialPath(argv[index]);

		if (!std::filesystem::is_directory(potentialPath)) return 1;

		slaggy::Log::output_path = potentialPath + "/";
	}
	
	return slaggy::LoggingNoRenderingProgram().run();
}
