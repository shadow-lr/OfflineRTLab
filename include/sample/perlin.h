#pragma once

#include "rtweekend.h"

class perlin {
public:
    perlin();

    // 获得噪声
    double noise(const point3 &p) const;
    // Turbulence(a composite noise that has multiple summed frequencies is used)
    double turb(const point3& p, int depth = 7) const;

    ~perlin();

private:
    static const int point_count = 256;
//    double *ranfloat;
    vec3* ranvec;
    int *perm_x;
    int *perm_y;
    int *perm_z;

    // get random sequence
    static int *perlin_generate_perm();

    // Perlin with trilienear interpolation
    static double trilinear_interp(double c[2][2][2], double u, double v, double w);

    static double perlin_interp(vec3 c[2][2][2], double u, double v, double w);

    // 洗牌算法
    static void permute(int *p, int n);
};
