#include <include/inputs.h>
#include <include/linalg.h>
#include <include/poisson.h>
#include <include/interpolate.h>

void test_interpolation() {
    char *filepath = "data/input-params.toml";
    InputData data = getInput(filepath);

    double x_min = vecGet(data.locs, 0);
    double x_max = vecGet(data.locs, data.locs.len - 1);
    Vec interp_points = generateInterPoints(x_min, x_max, 100); 

    // Perform interpolation using input locations and probabilities
    Vec interpolated_probs;
    lagrangeInterpolate(data.locs, data.probs, interp_points, &interpolated_probs);

    // Print results
    printf("\n=== Occupation Probability Interpolation ===\n");
    printf("Original points:\n");
    printf("x\t\tProbability\n");
    for(size_t i = 0; i < data.locs.len; i++) {
        printf("%e\t%f\n", vecGet(data.locs, i), vecGet(data.probs, i));
    }

    printf("\nInterpolated points:\n");
    printf("x\t\tProbability\n");
    for(size_t i = 0; i < interp_points.len; i++) {
        printf("%e\t%f\n", 
               vecGet(interp_points, i), 
               vecGet(interpolated_probs, i));
    }

    // Clean up
    freeVec(&interp_points);
    freeVec(&interpolated_probs);
    // Note: data.locs and data.probs are freed by the InputData cleanup
}