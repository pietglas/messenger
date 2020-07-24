#pragma once

#include <string>
#include "server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <thread>
#include <vector>

using std::thread;

class Server {
public:
	Server(uint16_t portnr);
	~Server();
	void acceptClientCalls();
private:
	int list_socket_;
	size_t nr_clients_ = 0;
	sockaddr_in hint_;
	std::vector<int> clients_;
	std::vector<thread> client_threads_;

	void receiveMessagesClient(int client_socket);
};