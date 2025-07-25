//Mô tả : Đây là một thư viện Perlin Noise đơn giản, gọn nhẹ của tác giả Reputeless.


// File: src/PerlinNoise.h (MỚI)
// Perlin Noise in C++
// Converted from Java reference implementation by Ken Perlin
#pragma once
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>

class PerlinNoise {
	// The permutation vector
	std::vector<int> p;
public:
	// Generate a new permutation vector based on the value of seed
	PerlinNoise(unsigned int seed);
	// Get a noise value, for 2D images this is usually z=0
	double noise(double x, double y, double z);
private:
	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);
};

PerlinNoise::PerlinNoise(unsigned int seed) {
	p.resize(256);
	// Fill p with values from 0 to 255
	std::iota(p.begin(), p.end(), 0);
	// Initialize a random engine with the seed
	std::default_random_engine engine(seed);
	// Suffle  the permutation vector
	std::shuffle(p.begin(), p.end(), engine);
	// Duplicate the permutation vector to avoid buffer overflow
	p.insert(p.end(), p.begin(), p.end());
}

double PerlinNoise::noise(double x, double y, double z) {
	// Find the unit cube that contains the point
	int X = (int)floor(x) & 255;
	int Y = (int)floor(y) & 255;
	int Z = (int)floor(z) & 255;

	// Find the relative x,y,z of the point in the cube
	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	// Compute fade curves for each of x,y,z
	double u = fade(x);
	double v = fade(y);
	double w = fade(z);

	// Hash coordinates of the 8 cube corners
	int A = p[X] + Y;
	int AA = p[A] + Z;
	int AB = p[A + 1] + Z;
	int B = p[X + 1] + Y;
	int BA = p[B] + Z;
	int BB = p[B + 1] + Z;

	// Add blended results from 8 corners of the cube
	double res = lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z), grad(p[BA], x - 1, y, z)), lerp(u, grad(p[AB], x, y - 1, z), grad(p[BB], x - 1, y - 1, z))), lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1), grad(p[BA + 1], x - 1, y, z - 1)), lerp(u, grad(p[AB + 1], x, y - 1, z - 1), grad(p[BB + 1], x - 1, y - 1, z - 1))));
	return (res + 1.0) / 2.0; // Return value in range [0, 1]
}

double PerlinNoise::fade(double t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::lerp(double t, double a, double b) {
	return a + t * (b - a);
}

double PerlinNoise::grad(int hash, double x, double y, double z) {
	int h = hash & 15;
	// Convert lower 4 bits of hash into 12 gradient directions
	double u = h < 8 ? x : y,
		v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}