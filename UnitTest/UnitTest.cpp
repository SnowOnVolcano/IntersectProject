#include "pch.h"
#include "CppUnitTest.h"
#include "../IntersectProject/head.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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
		(line.a * tLine.c - tLine.a * +line.c) / (float)D
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

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		
		TEST_METHOD(TestLine1)
		{
			points.clear();
			Line line = { 1, 2, 3 };
			Assert::AreEqual(1, line.a);
			Assert::AreEqual(2, line.b);
			Assert::AreEqual(3, line.c);
		}
		TEST_METHOD(TestLine2)
		{
			points.clear();
			Line line = { 1, 2, 3, 4 };
			Assert::AreEqual(-2, line.a);
			Assert::AreEqual(2, line.b);
			Assert::AreEqual(-2, line.c);
		}
		TEST_METHOD(TestLineLine1)
		{
			points.clear();
			Line l1 = { 1, 2, 3 };
			Line l2 = { -1, 5, -10 };
			calLineLineIst(l1, l2);
			Point p;
			for (Point it : points) {
				p = it;
			}
			Assert::AreEqual((float)-5.0, p.x);
			Assert::AreEqual((float)1.0, p.y);
		}
		TEST_METHOD(TestLineLine2)
		{
			points.clear();
			Line l1 = { 1, 2, 3 };
			Line l2 = { -2, -4, -4 };
			calLineLineIst(l1, l2);
			
			Assert::AreEqual(0, (int)points.size());
		}
		TEST_METHOD(TestLineLine3)
		{
			points.clear();
			Line l1 = { 0, -1, -2, 0 };
			Line l2 = { 1, -2, -3, 0 };
			calLineLineIst(l1, l2);

			Assert::AreEqual(0, (int)points.size());
		}
		TEST_METHOD(TestLineLine4)
		{
			points.clear();
			Line l1 = { -5, 1, 0, 2 };
			Line l2 = { -5, 1, -5, 0 };
			Line l3 = { -5, 1, -10, 0 };
			Line l4 = { -5, 1, 0, 1 };
			calLineLineIst(l1, l2);
			calLineLineIst(l1, l3);
			calLineLineIst(l1, l4);
			calLineLineIst(l2, l3);
			calLineLineIst(l2, l4);
			calLineLineIst(l3, l4);
			Assert::AreEqual(1, (int)points.size());
		}
		TEST_METHOD(TestLineLine5)
		{
			points.clear();
			Line l1 = { -5, 1, 0, 2 };
			Line l2 = { -5, 1, -5, 0 };
			Line l3 = { -5, 1, -10, 0 };
			Line l4 = { -5, 1, 0, 1 };
			Line l5 = { -34, 6, 0, 0 };
			calLineLineIst(l1, l2);
			calLineLineIst(l1, l3);
			calLineLineIst(l1, l4);
			calLineLineIst(l1, l5);
			calLineLineIst(l2, l3);
			calLineLineIst(l2, l4);
			calLineLineIst(l2, l5);
			calLineLineIst(l3, l4);
			calLineLineIst(l3, l5);
			calLineLineIst(l4, l5);
			Assert::AreEqual(4, (int)points.size());
		}
		TEST_METHOD(TestLineLine6)
		{
			points.clear();
			Line l1 = { -1000, -1000, 999, 999 };
			Line l2 = { -999, -998, 1000, 1000 };
			calLineLineIst(l1, l2);
			Assert::AreEqual(1, (int)points.size());
		}
		TEST_METHOD(TestLineLine7)
		{
			points.clear();
			Line l1 = { -998, -1000, 999, 999 };
			Line l2 = { -999, -998, 1000, 1000 };
			Line l3 = { -999, -998, 996, 1000 };
			calLineLineIst(l1, l2);
			calLineLineIst(l1, l3);
			calLineLineIst(l2, l3);
			Assert::AreEqual(3, (int)points.size());
		}
		TEST_METHOD(TestLineLine8)
		{
			points.clear();
			Line l1 = { 0, -1000, 0, 999 };
			Line l2 = { 0, -1000, 1, 1000 };
			Line l3 = { 1, -1000, 0, 999 };
			calLineLineIst(l1, l2);
			calLineLineIst(l1, l3);
			calLineLineIst(l2, l3);
			Assert::AreEqual(3, (int)points.size());
		}
	};
}
