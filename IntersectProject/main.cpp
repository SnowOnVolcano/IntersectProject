#include "head.h"

using namespace std;

// calculate the intersections of two lines
static void calLineLineIst(Line& line1, Line& line2) {
	int D;
	
	D = line1.a * line2.b - line2.a * line1.b;

	switch (D)
	{
	case 0:	// parallel
		break;
	default:
		//	line1: a1*x+b1*x+c1=0, line2: a2*x+b2*x+c2=0
		//	==> x=(b1*c2-b2*c1)/(a1*b2-a2*b1),
		//		y=(a2*c1-a1*c2)/(a1*b2-a2*b1)
		//	let D=a1*b2-a2*b1
		//	==> x=(b1*c2-b2*c1)/D, y=(a2*c1-a1*c2)/D
		Point point = { 
			(line1.b * line2.c - line2.b * line1.c) / (float)D, 
			(line2.a * line1.c - line1.a * line2.c) / (float)D 
		};
		points.insert(point);
		break;
	}
}

// calculate the intersections of line and Circle
static void calLineCircleIst(Line& line, Circle& circle) {
	int intercept;
	
	// intercept=r^2-d^2=r^2-(ax+by+c)^2/(a^2+b^2)
	intercept = (int)(pow(circle.r, 2) - 
		pow(line.a * circle.x + line.b * circle.y + line.c, 2) / (pow(line.a, 2) + pow(line.b, 2)));

	// not intersect
	if (intercept < 0) {
		return;
	}

	// tLine is perpendicular to line
	Line tLine = {
		line.b,
		-line.a,
		line.a * circle.y - line.b * circle.x
	};

	int D;
	D = tLine.a * line.b - line.a * tLine.b;

	// tPoint is the intersection of line and tLine
	Point tPoint = {
		(tLine.b * line.c - line.b * tLine.c) / (float)D,
		(line.a * tLine.c - tLine.a* + line.c) / (float)D
	};
	
	switch (intercept) 
	{
	case 0:	// line is tangent to circle
		points.insert(tPoint);
		break;

	default:// line passes through circle
		float vecX;
		float vecY;
		float offset;

		// (vecX, vecY) is a unit vector
		vecX = (float)(line.b / sqrt(pow(line.a, 2) + pow(line.b, 2)));
		vecY = (float)(-line.a / sqrt(pow(line.a, 2) + pow(line.b, 2)));
		
		// Offset is half of the intercept
		offset = (float)sqrt(intercept / (pow(line.a, 2) + pow(line.b, 2)));

		// intersection = tPoint +/- vec*offset
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

// calculate intersections of two circles
static void calCircleCircleIst(Circle& circle1, Circle& circle2) {
	int radiusSum;
	int radiusDiff;
	int centerDis;

	radiusSum = (int)pow(circle1.r + circle2.r, 2);
	radiusDiff = (int)pow(circle1.r - circle2.r, 2);
	centerDis = (int)(pow(circle1.x - circle2.x, 2) + pow(circle1.y - circle2.y, 2));
	
	// not intersect
	if (centerDis > radiusSum || centerDis < radiusDiff) {
		return;
	}

	// line passes both two intersections of circles
	Line line = {
		circle1.d - circle2.d, 
		circle1.e - circle2.e,
		circle1.f - circle2.f
	};
	
	// the intersections of two circles are also the intersections of line and circle
	calLineCircleIst(line, circle1);
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
	Line line;
	Circle circle;
	vector<Line> lines;
	vector<Circle> circles;
	
	fileIn >> N;

	for (int i = 0; i < N; i++) {
		fileIn >> type;
		switch (type)
		{
		case 'L':
			fileIn >> x1 >> y1 >> x2 >> y2;
			line = Line(x1, y1, x2, y2);
			for (Line it : lines) {
				calLineLineIst(line, it);
			}
			for (Circle it : circles) {
				calLineCircleIst(line, it);
			}
			lines.emplace_back(line);
			break;
		case 'C':
			fileIn >> x >> y >> r;
			circle = Circle(x, y, r);
			for (Line it : lines) {
				calLineCircleIst(it, circle);
			}
			for (Circle it : circles) {
				calCircleCircleIst(it, circle);
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