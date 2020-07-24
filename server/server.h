#pragma once

#include <string>
#include "server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

class Server {
public:
	Server(uint16_t portnr);
	void acceptClientCalls();
private:
	int list_socket_;
	sockaddr_in hint_;
};