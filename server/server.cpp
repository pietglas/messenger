#include "server.h"
#include <iostream>

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

void Server::acceptClientCalls() {
	if (listen(list_socket_, SOMAXCONN) == -1) {
		cerr << "Can't listen" << endl;
		return;
	}
	sockaddr_in client;
	socklen_t client_size = sizeof(client);
	char host[NI_MAXHOST];	// for hostname
	char svc[NI_MAXSERV];	// for servername

	// accept incoming calls from clients
	int client_socket = accept(list_socket_,
								(sockaddr*)&client,
								&client_size);

	if (client_socket == -1) {
		cerr << "Couldn't connect with client" << endl;
		return;
	}
	// free up the listening socket so it can take incoming calls again
	close(list_socket_);
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
		// echo the message back to the client
		send(client_socket, buff, bytes_rec + 1, 0);
	}
}