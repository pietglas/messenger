#include "server.h"
#include <iostream>

int main(int argc, char** argv) {
	if (argc == 1) {
		std::cerr << "No portnumber provided" << std::endl;
		return -1;
	}
	Server server{atoi(argv[1])};
	server.acceptClientCalls();
}