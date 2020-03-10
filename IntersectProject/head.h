#pragma once
#include <iostream>
#include <fstream>
#include <set>
#include <cmath>
#include <vector>
#include <set>

#define EPS 1e-7

// Point: (x, y)
struct Point
{
	double x, y;

	Point() :x(), y() {}

	Point(double x, double y) :x(x), y(y) {}

	bool operator < (const Point& other) const {
		if (fabs(x - other.x) <= EPS && fabs(y - other.y) <= EPS) {
			return false;
		}
		else {
			return (x != other.x) ? x < other.x : y < other.y;
		}
	}
	
};

// Line: ax + by + c = 0 (a^2+b^2!=0)
struct Line
{
	int a, b, c;

	Line() :a(), b(), c() {}

	Line(int a, int b, int c) :a(a), b(b), c(c) {}

	// ax1+by1+c=0, ax2+by2+c=0 
	// ==> a=y1-y2, b=x2-x1, c=x1*y2-x2*y1
	Line(int x1, int y1, int x2, int y2) :
		a(y1 - y2),
		b(x2 - x1),
		c(x1* y2 - x2 * y1)
	{}

};

// Circle: (x-x0)^2 + (y-y0)^2 = r0^2, x^2 + y^2 + dx + ey + f = 0
struct Circle
{
	int x, y, r;
	int d, e, f;

	Circle() :x(), y(), r(), d(), e(), f() {}

	// (x-x0)^2+(y-y0)^2=r0^2, x^2+y^2+dx+ey+f=0
	// ==> d=-2*x0, e=-2*y0, f=x0^2+y0^2-r0^2
	Circle(int x0, int y0, int r0) :
		x(x0),
		y(y0),
		r(r0),
		d(-2 * x0),
		e(-2 * y0),
		f((int)(pow(x0, 2) + pow(y0, 2) - pow(r0, 2)))
	{}

};

// set of intersections
std::set<Point> points;

// calculate the intersections of two lines
static void calLineLineIst(Line& line1, Line& line2);

// calculate the intersections of line and Circle
static void calLineCircleIst(Line& line, Circle& circle);

// calculate intersections of two circles
static void calCircleCircleIst(Circle& circle1, Circle& circle2);

