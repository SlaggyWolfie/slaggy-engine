#include "Random.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>

namespace slaggy
{
	bool Random::_isSetup = false;

	float Random::range(const float min, const float max, int decimalAccuracy)
	{
		if (!_isSetup) setup();

		while (std::pow(10, decimalAccuracy) >= RAND_MAX) decimalAccuracy /= 2;
		const float decimal = static_cast<float>(std::pow(10, decimalAccuracy));

		if (fabs(max - min) <= 1 / decimal)
		{
			std::cout << "Random Range Max and Min shouldn't be the same (or they're too close as float values)." << std::endl;
			return 0;
		}

		const int random = std::rand();

		const float result = fmod(random / decimal, max - min) + min;
		return result;
	}

	int Random::range(const int min, const int max)
	{
		if (!_isSetup) setup();

		if (max == min)
		{
			std::cout << "Random Range Max and Min shouldn't be the same." << std::endl;
			return 0;
		}

		return rand() % (max - min) + min;
	}

	float Random::value()
	{
		return range(0.0f, 1.0f);
	}

	void Random::setSeed(const unsigned seed)
	{
		srand(seed);
		_isSetup = true;
	}

	void Random::defaultSeed()
	{
		setup();
	}

	void Random::setup()
	{
		srand(static_cast<unsigned>(time(nullptr)));
		_isSetup = true;
	}
}