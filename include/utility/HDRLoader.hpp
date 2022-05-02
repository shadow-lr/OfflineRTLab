#pragma once

#include <iostream>

class HDRData
{
public:
	HDRData() : width(0), height(0), cols(nullptr)
	{
	}

	~HDRData()
	{
		delete cols;
	}

	int width, height;
	// each pixel takes 3 float32, each component can be of any value...
	float* cols;
};

class HDRLoader
{
public:
	static HDRData* load(const char* fileName);
};
