#include <math.h>

#define PI 3.141592653589793238462643383279502884

ref class NormalizedDistribution {
private:
	double math_exp;
	double sqrt_dispersion;
public:
	NormalizedDistribution(double math_exp, double sqrt_dispersion) {
		this->math_exp = math_exp;
		this->sqrt_dispersion = sqrt_dispersion;
	}

	double getDensity(double x) {
		return 1.0 / (sqrt_dispersion * sqrt(2.0 * PI))*exp(-((x - math_exp) * (x - math_exp)) / (2.0 * sqrt_dispersion * sqrt_dispersion));
	}
};

ref class CalculaterProbability {
private:
	float a;
	float b;
	NormalizedDistribution^ distance_probility;
	NormalizedDistribution^ angle_probility;

public:
	CalculaterProbability(float a, float b, NormalizedDistribution^ distance_probility, NormalizedDistribution^ angle_probility){
		this->distance_probility = distance_probility;
		this->angle_probility = angle_probility;
		this->a = a;
		this->b = b;
	}

	double getCurrentValue(double angle, double distance) {
		return a * distance_probility->getDensity(distance) + b * angle_probility->getDensity(angle);
	}
};