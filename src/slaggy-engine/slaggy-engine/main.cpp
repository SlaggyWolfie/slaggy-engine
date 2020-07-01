#include "programs/HelloTriangleProgram.hpp"
#include "programs/OctreeProgram.hpp"
#include "programs/CoreTestProgram.hpp"


int main()
{
	return slaggy::OctreeProgram().run();
	return slaggy::CoreTestProgram().run();
	return slaggy::HelloTriangleProgram().run();
}
