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

	bool operator < (const Point& other) const {
		return x < other.x;
	}
};

struct Line
{
	int a;
	int b;
	int c;
};

struct Circle
{
	int x;
	int y;
	int r;
	int d;
	int e;
	int f;
};

void initLine(Line& line, int x1, int y1, int x2, int y2) {
	line.a = y1 - y2;
	line.b = x2 - x1;
	line.c = x1 * y2 - x2 * y1;
}

void initCircle(Circle& circle, int x, int y, int r) {
	circle.x = x;
	circle.y = y;
	circle.r = r;
	circle.d = -2 * x;
	circle.e = -2 * y;
	circle.f = pow(x, 2) + pow(y, 2) - pow(r, 2);
}

void calLineLineIst(set<Point> &points, Line& line1, Line& line2) {
	int D;
	Point point;

	D = line1.a * line2.b - line2.a * line1.b;
	switch (D)
	{
	case 0:
		break;
	default:
		point.x = (line1.b * line2.c - line2.b * line1.c) / (float)D;
		point.y = (line1.b * line2.c - line2.b * line1.c) / (float)D;
		points.insert(point);
		break;
	}
}

void calLineCircleIst(set<Point>& points, Line& line, Circle& circle) {
	int intercept;

	intercept = pow(circle.r, 2) * (pow(line.a, 2) + pow(line.b, 2)) - pow(line.a * circle.x + line.b * circle.y + line.c, 2);

	if (intercept < 0) {
		return;
	}

	Line tLine;
	int D;
	Point tPoint;

	tLine.a = line.b;
	tLine.b = -line.a;
	tLine.c = line.a * circle.y - line.b * circle.x;

	D = tLine.a * line.b - line.a * tLine.b;
	tPoint.x = (tLine.b * line.c - line.b * tLine.c) / (float)D;
	tPoint.y = (tLine.b * line.c - line.b * tLine.c) / (float)D;
	
	switch (intercept)
	{
	case 0:
		points.insert(tPoint);
		break;
	default:
		Point e; 
		Point ist1;
		Point ist2;
		e.x = line.b / sqrt(pow(line.a, 2) + pow(line.b, 2));
		e.y = -line.a / sqrt(pow(line.a, 2) + pow(line.b, 2));
		intercept = sqrt(intercept / (pow(line.a, 2) + pow(line.b, 2)));
		ist1.x = tPoint.x + e.x * intercept;
		ist1.y = tPoint.y + e.y * intercept;
		ist2.x = tPoint.x - e.x * intercept;
		ist2.y = tPoint.y - e.y * intercept;
		points.insert(ist1);
		points.insert(ist2);
		break;
	}
}

void calCircleCircleIst(set<Point>& points, Circle& circle1, Circle& circle2) {
	int radiusSum;
	int radiusDiff;
	int centerDis;
	
	radiusSum = pow(circle1.r + circle2.r, 2);
	radiusDiff = pow(circle1.r - circle2.r, 2);
	centerDis = pow(circle1.x - circle2.x, 2) + pow(circle1.y - circle2.y, 2);
	
	if (centerDis > radiusSum || centerDis < radiusDiff) {
		return;
	}

	Line line;
	line.a = circle1.d - circle2.d;
	line.b = circle1.e - circle2.e;
	line.c = circle1.f - circle2.f;
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

	fileIn >> N;

	for (int i = 0; i < N; i++) {
		fileIn >> type;
		switch (type)
		{
		case 'L':
			Line line;
			fileIn >> x1 >> y1 >> x2 >> y2;
			initLine(line, x1, y1, x2, y2);
			for (Line it : lines) {
				calLineLineIst(points, line, it);
			}
			for (Circle it : circles) {
				calLineCircleIst(points, line, it);
			}
			lines.emplace_back(line);
			break;
		case 'C':
			Circle circle;
			fileIn >> x >> y >> r;
			initCircle(circle, x, y, r);
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