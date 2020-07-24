#include "server.h"
#include <iostream>
#include <sys/time.h>

using std::cout;
using std::endl;
using std::cerr;


Server::Server(uint16_t portnr) {
	// set listening socket
	list_socket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (list_socket_ == -1)
		std::cerr << "Can't create listening socket" << std::endl;
	// bind socket to IP / port. 
	hint_.sin_family = AF_INET;
	hint_.sin_port = htons(portnr);
	inet_pton(AF_INET, "0.0.0.0", &hint_.sin_addr);
	if (bind(list_socket_, (sockaddr*)&hint_, sizeof(hint_)) == -1) {
		cerr << "Can't bind to IP/port" << endl;
	}

}

Server::~Server() {
	for (auto& client_thread : client_threads_)
		client_thread.join();
}

void Server::acceptClientCalls() {
	do {
		if (listen(list_socket_, SOMAXCONN) == -1) {
			cerr << "Can't listen" << endl;
			return;
		}
		sockaddr_in client;
		socklen_t client_size = sizeof(client);
		char host[NI_MAXHOST];	// for hostname
		char svc[NI_MAXSERV];	// for servername

		// set timeout 
		struct timeval tv;
		tv.tv_sec = 20;
		tv.tv_usec = 0;
		fd_set rfds;
		FD_ZERO(&rfds);
		FD_SET(list_socket_, &rfds);
		int client_socket;
		// accept incoming calls from clients
		if (select(list_socket_+1, &rfds, NULL, NULL, &tv) > 0) {
			client_socket = accept(list_socket_,
										(sockaddr*)&client,
										&client_size);
		}
		else
			continue;

		if (client_socket == -1) {
			cerr << "Couldn't connect with client" << endl;
			return;
		}
		// keep track of the nr of clients
		++nr_clients_;
		// free up the listening socket so it can take incoming calls again
		// close(list_socket_);
		// clean up
		memset(host, 0, NI_MAXHOST);
		memset(svc, 0, NI_MAXSERV);
		// get name info of the client
		int result = getnameinfo((sockaddr*)&client,
									sizeof(client),
									host,
									NI_MAXHOST,
									svc,
									NI_MAXSERV,
									0);
		if (result) {
			cout << host << " connected on " << svc << endl;
		}
		else {
			inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
			cout << host << " connected on " << ntohs(client.sin_port) << endl;
		}
		// add client to list
		clients_.push_back(client_socket);
		// wait for messages until client disconnects
		client_threads_.push_back(
			thread(&Server::receiveMessagesClient, this, client_socket));
	} while (nr_clients_ > 0);
}

void Server::receiveMessagesClient(int client_socket) {
	// while receiving, echo message
	char buff[4096];
	while (1) {
		// clear buffer
		memset(buff, 0, 4096);
		// wait for a message
		int bytes_rec = recv(client_socket, buff, 4096, 0);
		if (bytes_rec == -1) {
			cerr << "There was a connection issue" << std::endl;
			break;
		}
		if (bytes_rec == 0) {
			cout << "The client disconnected" << std::endl;
			break;
		}
		// echo the message back to the clients
		for (auto& client : clients_)
			send(client, buff, bytes_rec + 1, 0);
	}
	// update number of clients
	--nr_clients_;
}