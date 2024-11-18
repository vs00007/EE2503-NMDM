#include <include/linalg.h>

// using double precision

static double lagrangeBasis(const Vec gridPoints, size_t i, double x)
{
    double basis = 1.0;

    for (size_t j = 0; j < gridPoints.len; j++)
    {
        if (j != i)
        {
            basis *= (x - vecGet(gridPoints, j)) /
                     (vecGet(gridPoints, i) - vecGet(gridPoints, j));
        }
    }
    return basis;
}

int lagrangeInterpolate(const Vec gridPoints, const Vec values, const Vec interPoints, Vec *result)
{
    if (result->len != interPoints.len)
    {
        *result = vecInitZerosA(interPoints.len);
    }

    for (size_t k = 0; k < interPoints.len; k++)
    {
        double x = vecGet(interPoints, k);
        double interpolated_value = 0.0;

        for (size_t i = 0; i < gridPoints.len; i++)
        {
            interpolated_value += vecGet(values, i) *
                                  lagrangeBasis(gridPoints, i, x);
        }

        VEC_INDEX(*result, k) = interpolated_value;
    }

    return LINALG_OK;
}

// keep in mind that this generate uniform mesh points
Vec generateInterPoints(double start, double end, size_t num_points)
{
    Vec points = vecInitZerosA(num_points);
    double step = (end - start) / (num_points - 1);

    for (size_t i = 0; i < num_points; i++)
    {
        VEC_INDEX(points, i) = start + i * step;
    }

    return points;
}