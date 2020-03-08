#include <iostream>
#include <fstream>

using namespace std;



int main(int argc, char* argv[]) {
	std::ifstream fileIn;
	std::ofstream fileOut;
	for (int i = 0; i < argc; i++) {
		if (strncmp(argv[i], "-i", 2) == 0) {
			fileIn = ifstream(argv[i + 1]);
		}
		else if (strncmp(argv[i], "-o", 2) == 0) {
			fileOut = ofstream(argv[i + 1]);
		}
	}
}