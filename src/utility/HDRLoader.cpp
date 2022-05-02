/***********************************************************************************
	Created:    17:9:2002
	FileName:     hdrloader.cpp
	Author:        Igor Kravtchenko

	Info:        Load HDR image and convert to a set of float32 RGB triplet.
************************************************************************************/

/*
	
	origin code : from https://www.flipcode.com/archives/HDR_Image_Reader.shtml
	This is a modified version of the original code. Addeed code to build marginal & conditional densities for IBL importance sampling
*/

#include "utility/HDRLoader.hpp"

#include <math.h>
#include <memory.h>
#include <stdio.h>

#include "geometry/ray.h"

using RGBE = unsigned char[4];
#define R            0
#define G            1
#define B            2
#define E            3

#define  MINELEN    8                // minimum scanline length for encoding
#define  MAXELEN    0x7fff            // maximum scanline length for encoding

static void workOnRGBE(RGBE *scan, int len, float *cols);
static bool decrunch(RGBE *scanline, int len, FILE *file);
static bool oldDecrunch(RGBE *scanline, int len, FILE *file);

float Luminance(const vec3 &c)
{
	return c[0] * 0.3f + c[1] * 0.6f + c[2] * 0.1f;
}

int LowerBound(const float *array, int lower, int upper, const float value)
{
	while (lower < upper)
	{
		int mid = lower + (upper - lower) / 2;

		if (array[mid] < value)
		{
			lower = mid + 1;
		}
		else
		{
			upper = mid;
		}
	}

	return lower;
}

HDRData *HDRLoader::load(const char *fileName)
{
	int i;
	char str[200];
	FILE *file;

	file = fopen(fileName, "rb");
	if (!file)
		return nullptr;

	auto res = new HDRData;

	fread(str, 10, 1, file);
	// Determine the file encoding type
	if (memcmp(str, "#?RADIANCE", 10))
	{
		fclose(file);
		return nullptr;
	}

	fseek(file, 1, SEEK_CUR);

	char cmd[200];
	i = 0;
	char c = 0, oldc;
	// Read useless information and discard it
	while (true)
	{
		oldc = c;
		c = fgetc(file);
		if (c == 0xa && oldc == 0xa)
			break;
		cmd[i++] = c;
	}

	char reso[200];
	i = 0;
	// Read image length and width
	while (true)
	{
		c = fgetc(file);
		reso[i++] = c;
		if (c == 0xa)
			break;
	}

	int w, h;
	if (!sscanf_s(reso, "-Y %d +X %d", &h, &w))
	{
		fclose(file);
		return nullptr;
	}

	res->width = w;
	res->height = h;

	//定义图像数据数组
	auto cols = new float[w * h * 3];
	res->cols = cols;

	auto scanline = new RGBE[w];
	if (!scanline)
	{
		fclose(file);
		return nullptr;
	}

	// convert image 
	for (int y = h - 1; y >= 0; y--)
	{
		if (decrunch(scanline, w, file) == false)
			break;
		workOnRGBE(scanline, w, cols);
		cols += w * 3;
	}

	delete[] scanline;
	fclose(file);

	//	buildDistributions(res);
	return res;
}

/*
The HDRI file three-component data is restored to the scene three-component data
*/
float convertComponent(int expo, int val)
{
	float v = val / 256.0f;
	float d = static_cast<float>(pow(2, expo));
	return v * d;
}

/*
The RGBE components of HDRI files are calculated
*/
void workOnRGBE(RGBE *scan, int len, float *cols)
{
	while (len-- > 0)
	{
		int expo = scan[0][E] - 128;
		cols[0] = convertComponent(expo, scan[0][R]);
		cols[1] = convertComponent(expo, scan[0][G]);
		cols[2] = convertComponent(expo, scan[0][B]);
		cols += 3;
		scan++;
	}
}

/*
Read each row of data
*/
bool decrunch(RGBE *scanline, int len, FILE *file)
{
	int i, j;

	// Check whether it is larger than two bytes
	if (len < MINELEN || len > MAXELEN)
		return oldDecrunch(scanline, len, file);

	i = fgetc(file);
	if (i != 2)
	{
		fseek(file, -1, SEEK_CUR);
		return oldDecrunch(scanline, len, file);
	}

	// If there are only two bytes left, the default is GB
	scanline[0][G] = fgetc(file);
	scanline[0][B] = fgetc(file);
	i = fgetc(file);

	if (scanline[0][G] != 2 || scanline[0][B] & 128)
	{
		scanline[0][R] = 2;
		scanline[0][E] = i;
		return oldDecrunch(scanline + 1, len - 1, file);
	}

	// read each component
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < len;)
		{
			unsigned char code = fgetc(file);
			if (code > 128)
			{
				// run
				code &= 127;
				unsigned char val = fgetc(file);
				while (code--)
					scanline[j++][i] = val;
			}
			else
			{
				// non-run
				while (code--)
					scanline[j++][i] = fgetc(file);
			}
		}
	}

	return feof(file) ? false : true;
}

bool oldDecrunch(RGBE *scanline, int len, FILE *file)
{
	int i;
	int rshift = 0;

	// Read RGBE four-component data
	while (len > 0)
	{
		scanline[0][R] = fgetc(file);
		scanline[0][G] = fgetc(file);
		scanline[0][B] = fgetc(file);
		scanline[0][E] = fgetc(file);
		if (feof(file))
			return false;

		if (scanline[0][R] == 1 &&
			scanline[0][G] == 1 &&
			scanline[0][B] == 1)
		{
			for (i = scanline[0][E] << rshift; i > 0; i--)
			{
				memcpy(&scanline[0][0], &scanline[-1][0], 4);
				scanline++;
				len--;
			}
			rshift += 8;
		}
		else
		{
			scanline++;
			len--;
			rshift = 0;
		}
	}
	return true;
}
