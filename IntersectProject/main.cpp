#include <iostream>
#include <fstream>
#include <set>
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

void initLine(Line& line, int x1, int y1, int x2, int y2) {
	line.a = y1 - y2;
	line.b = x2 - x1;
	line.c = x1 * y2 - x2 * y1;
}

void calLineIst(set<Point> &points, Line& line1, Line& line2) {
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
	vector<Line> lines;
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
				calLineIst(points, line, it);
			}
			lines.emplace_back(line);
			break;
		default:
			break;
		}
	}

	fileOut << points.size();
	return 0;
}