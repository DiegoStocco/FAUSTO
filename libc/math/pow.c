#include <math.h>


double pow(double x, double y) {
	double res = x;	
	for (double i = 1; i < y; i++) {
		res *= x;
	}
	return res;
}
float powf(float x, float y) {
	float res = x;	
	for (int i = 1; i < y; i++) {
		res *= x;
	}
	return res;
}
long double powl(long double x, long double y) {
	long double res = x;	
	for (long double i = 1; i < y; i++) {
		res *= x;
	}
	return res;
}


