#include <iostream>
#include <fstream>
#include <set>
#include <cmath>
#include <vector>

using namespace std;

struct Point
{
	float x;
	float y;

	Point() :x(), y() {}

	Point(float x, float y) :x(x), y(y) {}

	bool operator < (const Point& other) const {
		return x < other.x;
	}
};

struct Line
{
	int a;
	int b;
	int c;

	Line() :a(),b(),c() {}

	Line(int a, int b, int c) :a(a), b(b), c(c) {}

	Line(int x1, int y1, int x2, int y2) :
		a(y1 - y2),
		b(x2 - x1),
		c(x1* y2 - x2 * y1)
	{}

};

struct Circle
{
	int x;
	int y;
	int r;
	int d;
	int e;
	int f;

	Circle() :x(), y(), r(), d(), e(), f(){}

	Circle(int x, int y, int r) :
		x(x), 
		y(y), 
		r(r), 
		d(-2 * x), 
		e(-2 * y), 
		f((int)(pow(x, 2) + pow(y, 2) - pow(r, 2))) 
	{}

};


void calLineLineIst(set<Point> &points, Line& line1, Line& line2) {
	int D;

	D = line1.a * line2.b - line2.a * line1.b;
	
	switch (D)
	{
	case 0:
		break;
	default:
		Point point = { 
			(line1.b * line2.c - line2.b * line1.c) / (float)D, 
			(line2.a * line1.c - line1.a * line2.c) / (float)D 
		};
		points.insert(point);
		break;
	}
}

void calLineCircleIst(set<Point>& points, Line& line, Circle& circle) {
	int intercept;

	intercept = (int)(pow(circle.r, 2) * (pow(line.a, 2) + pow(line.b, 2)) - pow(line.a * circle.x + line.b * circle.y + line.c, 2));

	if (intercept < 0) {
		return;
	}

	Line tLine = {
		line.b,
		-line.a,
		line.a * circle.y - line.b * circle.x
	};

	int D;
	D = tLine.a * line.b - line.a * tLine.b;

	Point tPoint = {
		(tLine.b * line.c - line.b * tLine.c) / (float)D,
		(line.a * tLine.c - tLine.a* + line.c) / (float)D
	};
	
	switch (intercept)
	{
	case 0:
		points.insert(tPoint);
		break;
	default:
		float vecX;
		float vecY;
		float offset;

		vecX = (float)(line.b / sqrt(pow(line.a, 2) + pow(line.b, 2)));
		vecY = (float)(-line.a / sqrt(pow(line.a, 2) + pow(line.b, 2)));
		offset = (float)sqrt(intercept / (pow(line.a, 2) + pow(line.b, 2)));

		Point ist1 = {
			tPoint.x + vecX * offset,
			tPoint.y + vecY * offset
		};
		Point ist2 = {
			tPoint.x - vecX * offset,
			tPoint.y - vecY * offset
		};
		points.insert(ist1);
		points.insert(ist2);
		break;
	}
}

void calCircleCircleIst(set<Point>& points, Circle& circle1, Circle& circle2) {
	int radiusSum;
	int radiusDiff;
	int centerDis;
	
	radiusSum = (int)pow(circle1.r + circle2.r, 2);
	radiusDiff = (int)pow(circle1.r - circle2.r, 2);
	centerDis = (int)(pow(circle1.x - circle2.x, 2) + pow(circle1.y - circle2.y, 2));
	
	if (centerDis > radiusSum || centerDis < radiusDiff) {
		return;
	}

	Line line = {
		circle1.d - circle2.d, 
		circle1.e - circle2.e,
		circle1.f - circle2.f
	};
	
	calLineCircleIst(points, line, circle1);
}

int main(int argc, char* argv[]) {
	ifstream fileIn;
	ofstream fileOut;
	/*for (int i = 0; i < argc; i++) {
		if ((string)argv[i] == "-i") {
			fileIn = ifstream(argv[i + 1]);
		}
		else if ((string)argv[i] == "-o") {
			fileOut = ofstream(argv[i + 1]);
		}
	}*/
	fileIn.open("input.txt");
	fileOut.open("output.txt");

	int N;
	char type;
	int x1, y1;
	int x2, y2;
	int x, y, r;
	vector<Line> lines;
	vector<Circle> circles;
	set<Point> points;

	Line line;
	Circle circle;
	
	fileIn >> N;

	for (int i = 0; i < N; i++) {
		fileIn >> type;
		switch (type)
		{
		case 'L':
			fileIn >> x1 >> y1 >> x2 >> y2;
			line = Line(x1, y1, x2, y2);
			for (Line it : lines) {
				calLineLineIst(points, line, it);
			}
			for (Circle it : circles) {
				calLineCircleIst(points, line, it);
			}
			lines.emplace_back(line);
			break;
		case 'C':
			fileIn >> x >> y >> r;
			circle = Circle(x, y, r);
			for (Line it : lines) {
				calLineCircleIst(points, it, circle);
			}
			for (Circle it : circles) {
				calCircleCircleIst(points, it, circle);
			}
			circles.emplace_back(circle);
			break;
		default:
			break;
		}
	}

	fileOut << points.size();
	return 0;
}