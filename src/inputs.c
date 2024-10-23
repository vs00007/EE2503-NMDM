/*
 * File Structure:
 * -------------
 * Line 1: Oxide parameters (4 values)
 * Line 2: Number of traps (1 value)
 * Line 3: Trap positions array
 * Line 4: Trap occupation probabilities array (Init to zero)
 */

/*
 * Line 1 Format - Oxide Parameters:
 * ------------------------------
 * <thickness> <rel_permittivity> <V_bottom> <V_top>
 * 
 * Example: 5e-9 3.9 0.0 1.0
 * 
 * Where:
 * - thickness: Oxide thickness in meters (e.g., 5e-9 for 5nm)
 * - rel_permittivity: Relative permittivity (e.g., 3.9 for SiO2)
 * - V_0 : Voltage at top electrode in Volts
 * - V_L: Voltage at bottom electrode in Volts
s*/

/*
 * Line 2 Format - Number of Traps:
 * -----------------------------
 * <num_traps>
 * 
 * Example: 5
 * 
 * Note: Must match the number of values in arrays below
 */

/*
 * Line 3 Format - Trap Positions:
 * ---------------------------
 * {x1, x2, x3, ..., xn}
 * 
 * Example: {1e-9, 2e-9, 3e-9, 4e-9, 4.5e-9}
 * 
 * Requirements:
 * - Each x_i must be between 0 and oxide thickness
 * - Number of values must match num_traps
 */

/*
 * Line 4 Format - Occupation Probabilities:
 * ------------------------------------
 * {p1, p2, p3, ..., pn}
 * 
 * Example: {0.0, 0.0, 0.0, 0.0, 0.0}
 * 
 * Requirements:
 * - Should be 0 initially
 * - Number of values must match num_traps
 */

/*
 * Complete Example:
 * --------------
 * 5e-9 3.9 0.0 1.0
 * 5
 * {1e-9, 2e-9, 3e-9, 4e-9, 4.5e-9}
 * {0, 0, 0, 0, 0}
 */

/*
 * Errors to Handle:
 * -------------------
 * 1. Missing or extra commas in arrays
 * 2. Trap positions outside oxide thickness
 * 3. Number of values not matching num_traps
 * 4. Invalid number formats (Check for nan)
 */

/*
 * Units:
 * ----
 * - Lengths: meters (m)
 * - Voltages: volts (V)
 * - Permittivity: dimensionless
 * - Probabilities: dimensionless
 */

/*
 * Note on Scientific Notation:
 * ------------------------
 * - Use e notation for small/large numbers
 * - Examples: 
 *   * 5e-9 means 5 × 10⁻⁹
 *   * 1e-2 means 0.01
 */

/*
 * Tips for Error-Free Input:
 * ----------------------
 * 1. Double-check number of traps matches array lengths
 * 2. Verify all trap positions are within oxide
 * 3. Ensure all probabilities are valid
 * 4. Use consistent number formatting
 * 5. Remove any extra whitespace or blank lines
 */