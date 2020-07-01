#pragma once
#ifndef RANDOM_HPP
#define RANDOM_HPP

namespace slaggy
{
	class Random
	{
	public:
		static float range(float min, float max, int decimalAccuracy = 6);
		static int range(int min, int max);
		
		static float value();
		
		static void setSeed(unsigned seed);
		static void defaultSeed();
		
	private:
		static void setup();
		static bool _isSetup;
	};
}
#endif