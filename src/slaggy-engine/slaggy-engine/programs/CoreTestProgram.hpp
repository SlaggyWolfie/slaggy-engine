#pragma once
#ifndef CORE_TEST_PROGRAM_HPP
#define CORE_TEST_PROGRAM_HPP

#include "Program.hpp"

namespace slaggy
{
	class CoreTestProgram : public Program
	{
	public:
		int run() override;
	};
}
#endif