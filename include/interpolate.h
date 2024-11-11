#pragma once

#include <include/linalg.h>

int lagrangeInterpolate(const Vec grid_points, const Vec values, 
                        const Vec interp_points, Vec* result);

Vec generateInterPoints(double start, double end, size_t num_points);