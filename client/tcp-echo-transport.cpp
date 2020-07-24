#include "tcp-echo-transport.h"
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

using std::cerr;
using std::cout;
using std::endl;

TCPEchoTransport::TCPEchoTransport(uint16_t portnr) :
	EchoTransport() {
	sock_ = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_ == -1) {
		std::cerr << "Can't create a socket." << std::endl;
	}
	// create hint structure for the server we're connecting with
	hint_.sin_family = AF_INET;
	hint_.sin_port = htons(portnr);
	inet_pton(AF_INET, ip_.c_str(), &hint_.sin_addr);
	// connect to the server
	int connect_sv = connect(sock_, (sockaddr*)&hint_, sizeof(hint_));
	if (connect_sv == -1) {
		cerr << "Couldn't connect to server" << endl;
	}
	else
		connected_ = true;
	await_answer_ = thread(&TCPEchoTransport::awaitAnswer, this);

	// let recv timeout after every second, to prevent the program getting
	// stuck when trying to exit.
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	int z = setsockopt(sock_, SOL_SOCKET, SO_RCVTIMEO, 
		(const char*)&tv, sizeof tv);
	// if an error occured, 
	if (z == -1) {
		cerr << "an error occured setting the socket" << endl;
		connected_ = false;
	}
}

TCPEchoTransport::~TCPEchoTransport() {
}

void TCPEchoTransport::send(const string& msg) {
	sendServer(msg);
}

void TCPEchoTransport::sendClient(const string& msg) {
	return_message_(msg);
}

void TCPEchoTransport::sendServer(const string& msg) {
	int send_msg = sendto(sock_, msg.c_str(), msg.size() + 1, 0, NULL, 0);
	if (send_msg == -1)
		cerr << "Server didn't receive the message" << endl;
}

void TCPEchoTransport::awaitAnswer() {
	char buff[4096];
	int server_msg;
	string msg;
	while (connected_) {
		memset(buff, 0, 4096);
		server_msg = recv(sock_, buff, 4096, 0);
		if (server_msg > 0) {
			msg = string(buff);
			sendClient(msg);
		}
	}
}

void TCPEchoTransport::disconnect() {
	close(sock_);
	connected_ = false;
	await_answer_.join();
}

void TCPEchoTransport::setPassMessage(PassMessage msg) {
	return_message_ = msg;
}