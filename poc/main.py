import numpy as np
from scipy.integrate import quad

# Constants
m_e = 9.10938356e-31  # Mass of electron (kg)
hbar = 1.0545718e-34  # Reduced Planck's constant (J·s)
k_B = 1.380649e-23    # Boltzmann constant (J/K)
E_F = 5.5e-19         # Fermi energy (example: ~3.4 eV = 5.5e-19 J)
T = 300               # Temperature in Kelvin

# Density of States function
def g(E):
    return ((2 * m_e)**1.5 / (2 * np.pi**2 * hbar**3)) * np.sqrt(E)

# Fermi-Dirac distribution
def f(E):
    return 1 / (np.exp((E - E_F) / (k_B * T)) + 1)

# Integrand for the number of electrons above Fermi level
def integrand(E):
    return g(E) * f(E)

# Integration to find number of electrons above Fermi level
N_above, _ = quad(integrand, E_F, np.inf)

print(f"Number of electrons above Fermi level per unit volume: {N_above:.3e} electrons/m^3")
