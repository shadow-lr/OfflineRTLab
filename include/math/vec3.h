#pragma once

#define RTLAB_SIMD_OPTIMIZE
#ifdef RTLAB_SIMD_OPTIMIZE
#include "vec3_avx.h"
#else
#include "vec3_scalar.h"
#endif