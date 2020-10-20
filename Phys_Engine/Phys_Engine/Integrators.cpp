#include <iostream>
#include <stdlib.h>

// Backwards Euler
// 1st order
void backwardsEuler(double& x, double& v, double a, double dt)
{
	x += v * dt; // Update position (Uses old velocity)
	v += a * dt; // Update velocity
}

// Forward (Symplectic) Euler
// 1st order
void forwardEuler(double& x, double& v, double a, double dt)
{
	v += a * dt; // Update velocity
	x += v * dt; // Update position (Uses new velocity)
}