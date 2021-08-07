# ConstGA
Uses C++ constant expressions to do Geometric Algebra. Removes all calculations that result in zeros at compile time. Multivector coefficients are stored in an array.
The coefficient basis blades are only known and used at compile time. At runtime the only thing stored are the coefficients.