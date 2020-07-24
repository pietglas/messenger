#pragma once

#include "echo-transport.h"
#include <functional>
#include <string>
#include <netinet/in.h>
#include <thread>

using std::thread;

using std::string;

class TCPEchoTransport : public EchoTransport {
public:
	TCPEchoTransport(uint16_t portnr);
	~TCPEchoTransport();
	void send(const string& message) override;
	void sendClient(const string& message) override;
	void setPassMessage(PassMessage msg) override;

	void awaitAnswer();
	void disconnect();
private:
	bool connected_ = false;
	int sock_;
	std::string ip_ = string("127.0.0.1");
	sockaddr_in hint_;
	PassMessage return_message_;
	thread await_answer_;

	void sendServer(const string& message);
};