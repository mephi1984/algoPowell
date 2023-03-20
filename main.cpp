#include <iostream>
#include <vector>
#include <array>
#include <cassert>
#include <chrono>
#include <algorithm>


//Variant 21
// f = sin(x) - x + 2.0
// x0 = 2.55
// Calc an integral function from it

double f(double x)
{
	return cos(x) + x * x / 2.0 - 2.0 * x;
}

double ff(double x)
{
	return -(sin(x) - x + 2.0);
}

double fff(double x)
{
	return -cos(x) + 1.0;
}

/*
double f(double x)
{
	return sin(x) - x + 2.0;
}

double ff(double x)
{
	return cos(x) - 1.0;
}

double fff(double x)
{
	return -sin(x);
}*/


double calcQuadraticApproximation(const std::array<std::pair<double, double>, 3>& points)
{
	double x1 = points[0].first;
	double x2 = points[1].first;
	double x3 = points[2].first;

	double fx1 = points[0].second;
	double fx2 = points[1].second;
	double fx3 = points[2].second;


	double a1 = (fx2 - fx1) / (x2 - x1);

	double a2 = ((fx3 - fx1) / (x3 - x1) - (fx2 - fx1) / (x2 - x1)) / (x3 - x2);

	double xopt = (x2 + x1) / 2.0 - (a1 / (2.0 * a2));

	return xopt;
}

size_t getMinPoint(const std::array<std::pair<double, double>, 3>& points)
{
	if ((points[0].second <= points[1].second) && (points[0].second <= points[2].second))
	{
		return 0;
	}
	else if ((points[1].second <= points[0].second) && (points[1].second <= points[2].second))
	{
		return 1;
	}
	else
	{
		return 2;
	}
}

size_t getMinPoint4(const std::array<std::pair<double, double>, 4>& points)
{
	if ((points[0].second <= points[1].second) && (points[0].second <= points[2].second) && (points[0].second <= points[3].second))
	{
		return 0;
	}
	else if ((points[1].second <= points[0].second) && (points[1].second <= points[2].second) && (points[1].second <= points[3].second))
	{
		return 1;
	}
	else if ((points[2].second <= points[0].second) && (points[2].second <= points[1].second) && (points[2].second <= points[3].second))
	{
		return 2;
	}
	else
	{
		return 3;
	}
}


void calcPowell(double x0)
{
	double dx = 1.0;
	double x1 = x0;
	double x2 = x1 + dx;

	double xopt;
	double fxopt;

	double eps = 0.0000000001;


	int m = 0;


	std::cout << "Going Powell:" << std::endl;

	double fx1 = f(x1);
	double fx2 = f(x2);

	// x1 .... x2

	double x3;

	if (fx1 > fx2)
	{
		x3 = x1 + 2.0 * dx;

		// x1 .... x2 .... x3

	}
	else
	{
		x3 = x1 - dx;

		// x3 .... x1 .... x2
	}

	double fx3 = f(x3);

	std::array<std::pair<double, double>, 3> points{
		std::pair<double, double>{x1, fx1},
		std::pair<double, double>{x2, fx2},
		std::pair<double, double>{x3, fx3} };

	std::sort(points.begin(), points.end(), [](std::pair<double, double> p1, std::pair<double, double> p2)
		{
			return p1.first < p2.first;
		});

	size_t minPointIndex;

	std::array<std::pair<double, double>, 3> pointsOld;

	do
	{
		minPointIndex = getMinPoint(points);

		xopt = calcQuadraticApproximation(points);
		fxopt = f(xopt);

		std::array<std::pair<double, double>, 4> points4{
			points[0],
			points[1],
			points[2],
			std::pair<double, double>{xopt, fxopt},
		};

		std::sort(points4.begin(), points4.end(), [](std::pair<double, double> p1, std::pair<double, double> p2)
			{
				return p1.first < p2.first;
			});

		size_t minPointIndex4 = getMinPoint4(points4);

		if ((minPointIndex4 == 0) || (minPointIndex4 == 3))
		{
			throw std::runtime_error("Minimum is outside the range");
		}


		pointsOld = points;
		

		points[0] = points4[minPointIndex4 - 1];
		points[1] = points4[minPointIndex4];
		points[2] = points4[minPointIndex4 + 1];

		m++;

		std::cout << "Iteration just pased: " << m
			<< " x1 = " << points[0].first
			<< " fx1 = " << points[0].second
			<< " x2 = " << points[1].first
			<< " fx2 = " << points[1].second
			<< " x3 = " << points[2].first
			<< " fx3 = " << points[2].second
			<< " xopt = " << xopt
			<< " fxopt = " << fxopt
			<< " eps1 = " << abs(xopt - pointsOld[minPointIndex].first)
			<< " eps2 = " << abs(fxopt - pointsOld[minPointIndex].second)
			<< std::endl;

	} while (abs(xopt - pointsOld[minPointIndex].first) > eps || abs(fxopt - pointsOld[minPointIndex].second) > eps);


	std::cout << " For eps = " << eps << " m = " << m << std::endl;
}


int main()
{
	double x0 = 2.55;

	calcPowell(x0);

}