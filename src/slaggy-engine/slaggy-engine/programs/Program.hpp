#pragma once
#ifndef PROGRAM_HPP
#define PROGRAM_HPP

class Program
{
public:
	Program() = default;
	virtual ~Program() = default; // destructor
	
	Program(const Program&) = delete; // copy constructor
	Program(const Program&&) = delete; // move constructor
	Program& operator= (const Program&) = delete; // copy assignment operator
	Program& operator= (Program&&) = delete; // move assignment operator
	
	virtual int run() = 0;
};
#endif