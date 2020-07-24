#include "client.h"
#include <iostream>
#include <stdlib.h>


int main(int argc, char** argv) {
	if (argc == 1) {
		std::cerr << "No portnumber provided" << std::endl;
		return -1;
	}
	Client client{tcp_echo, atoi(argv[1])};

	client.userInput();
}