#include "gradiant.h"
#include <cmath>

double
Gradiant::cosGradiant(double start, double end, double value) {
	if( start == end ) return value;
	double range = end - start;
	double progression = cos(value * M_PI);
	double move = progression * range;
	return start + move;
}

double
Gradiant::polGradiant(double start, double end, double value, double power) {
	if( start == end ) return value;
	double range = end - start;
	double progression = pow(value, power);
	double move = progression * range;
	return start + move;
}
