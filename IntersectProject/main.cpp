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
	
	return 0;
}