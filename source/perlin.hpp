#ifndef PERLIN_INCLUDED
#define PERLIN_INCLUDED

#include "util.hpp"

std::size_t const PERLIN_SIZE = 256;

class perlin
{
public:
	perlin();

	void Init(unsigned int nSeed);

	float Noise1(float x);
	float Noise2(float x, float y);
	float Noise3(float x, float y, float z);

protected:
	// Permutation table
	unsigned char p[PERLIN_SIZE];

	// Gradients
	float gx[PERLIN_SIZE];
	float gy[PERLIN_SIZE];
	float gz[PERLIN_SIZE];
};

#endif//PERLIN_INCLUDED

