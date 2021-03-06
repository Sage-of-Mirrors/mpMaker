#include <iostream>

#include "collisionfile.h"

void PrintHelpMessage() {
	std::cout << "mpMaker - A map collision tool for Luigi's Mansion." << std::endl;
	std::cout << "Written by Sage of Mirrors/Gamma (@SageOfMirrors) with the help of many others." << std::endl;
	std::cout << "Please report any issues to the GitHub repository at https://github.com/Sage-of-Mirrors/mpMaker/issues." << std::endl;
	std::cout << std::endl;
	std::cout << "Usage: mpMaker.exe input_file.obj" << std::endl;
	std::cout << "       mpMaker.exe input_file.mp" << std::endl;
}

int main(int argc, char *argv[])
{
	if (argc <= 1) {
		PrintHelpMessage();
		return 0;
	}

	CollisionFile col(argv[1]);
	col.Write("D:\\test.obj", FileType::obj);
}

