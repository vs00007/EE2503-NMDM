# Poisson Solver Library Documentation

## Overview

This library provides functionality for solving the one-dimensional Poisson equation for discrete point charges.



## Constants

The library uses several predefined constants (defined in header files):
- `K`: Coulomb's constant $\left(\dfrac{1}{4π\epsilon}\right)$
- `Q`: Unit charge constant
- `TOL`: Tolerance for minimum distance calculations (to avoid singularities)


## Structures

### OxParams
```c
typedef struct OxParams 
{
    long double V_0;    // Potential at x = 0 (Bottom electrode)
    long double V_L;    // Potential at x = L (Top electrode)
    long double eps_r;  // Relative permittivity of oxide
    long double L;      // Oxide thickness
} OxParams;
```
#### Parameters
1. **V_0 **

- Potential at the Top Electrode (x = 0)


2. **V_L**

- Potential at the Bottom Electrode (x = L)
- Should be zero, but hey, I don't judge

3. **eps_r**

- Relative permittivity (dielectric constant) of oxide
- Typical values:
    - SiO2: 3.9
    - HfO2: 25
    - Al2O3: 9

4. **L**

- Oxide thickness
- Typical range: 1-100 nm

> Necessary for calculation of Energy

## Core Functions

### analyticalPoissonSol

```c
long double analyticalPoissonSol(const Vec f_n, const Vec d, long double x)
```

Calculates the electric potential at a point due to a set of point charges.

#### Parameters:
- `f_n`: Vector containing charge magnitudes
- `d`: Vector containing charge positions
- `x`: Position at which to calculate the potential
- Returns: The calculated potential value

#### Implementation Details:
- Calculates potential using the formula: V = K·Q·Σ(qᵢ/|x - xᵢ|)
- Skips calculations for points too close to charges (within TOL)
- Does not perform input validation

### poissonSolve

```c
long double poissonSolve(const Vec f_n, const Vec d, long double x)
```

Safe wrapper for analyticalPoissonSol with input validation.

#### Parameters:
- `f_n`: Vector containing charge magnitudes
- `d`: Vector containing charge positions
- `x`: Position at which to calculate the potential
- Returns: The calculated potential value, or NAN if inputs are invalid

#### Error Handling:
- Returns NAN and sets errno to EINVAL for invalid inputs
- Validates vector lengths and checks for NaN values

### getGridV

```c
Vec getGridV(Vec f_n, Vec d)
```

Calculates potentials at each charge position.

#### Parameters:
- `f_n`: Vector containing charge magnitudes
- `d`: Vector containing charge positions
- Returns: Vector containing potential values at each charge position

#### Error Handling:
- Returns a null vector {NULL, 0, 0} for invalid inputs
- Validates input vectors for matching lengths and NaN values

## Helper Functions

### validateInput

```c
int validateInput(const Vec f_n, const Vec d, long double x)
```

Validates inputs for poissonSolve function.

#### Checks:
- Vector lengths match and are non-zero
- x is not NaN
- No NaN values in either vector
- Returns: 1 if valid, 0 if invalid

### validateTwoVecs

```c
int validateTwoVecs(const Vec f_n, const Vec d)
```

Validates two vectors for getGridV function.

#### Checks:
- Vector lengths match and are non-zero
- No NaN values in either vector
- Returns: 1 if valid, 0 if invalid


## Usage Example

```c
// Initialize vectors
Vec charges = vecInitA(0.0, 3);
Vec positions = vecInitA(0.0, 3);

// Set charge values
charges.x[0] = 1.0;
charges.x[1] = -2.0;
charges.x[2] = 1.5;

// Set positions
positions.x[0] = 1.0;
positions.x[1] = 2.0;
positions.x[2] = 3.0;

// Calculate potential at a point
long double potential = poissonSolve(charges, positions, 2.5);

// Calculate potentials at charge positions
Vec gridPotentials = getGridV(charges, positions);

// Do whatever with the potentials (Maybe masterSolve())

// Clean up
freeVec(&charges);
freeVec(&positions);
freeVec(&gridPotentials);
```

## Error Handling

The library uses the following error handling mechanisms:
1. Input validation through helper functions
2. Return of NAN for invalid inputs in poissonSolve
3. Return of null vectors for invalid inputs in getGridV
4. Setting of errno to EINVAL for invalid inputs
5. Skipping of calculations for points too close to charges

## Memory Management

- All vectors passed to functions should be properly initialized
- getGridV returns a new vector that must be freed by the caller
- Input vectors should be freed by the caller after use

## Limitations

1. One-dimensional calculations only
2. Discrete point charges only

## Performance Considerations

- O(n) complexity for single point calculations
- O(n²) complexity for grid calculations
