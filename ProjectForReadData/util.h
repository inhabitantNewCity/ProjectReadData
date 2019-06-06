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

ref class GeomUtill {
private: 
	static double max(double x, double y)
	{
		if (x < y) {
			return y;
		}
		return x;
	}

	static double min(double x, double y)
	{
		if (x > y) {
			return y;
		}
		return x;
	}
public:
	static bool isPointInhereLine(System::Windows::Point point, System::Windows::Shapes::Line^ line) {
		double k, c;

		if (line->X1 == line->X2) {
			return (point.X == line->X1 && point.Y >= min(line->Y1, line->Y2) && point.Y <= max(line->Y1, line->Y2));
		}

		k = (line->Y2 - line->Y1) / (line->X2 - line->X1);
		c = line->Y1 - k * line->X1;

		return (point.Y == point.X * k + c) 
			&& (((point.X >= line->X1 ) && (point.Y >= line->Y1) && (point.X <= line->X2) && (point.Y <= line->Y2))
				|| ((point.X <= line->X1) && (point.Y <= line->Y1) && (point.X >= line->X2) && (point.Y >= line->Y2)));
	}
};