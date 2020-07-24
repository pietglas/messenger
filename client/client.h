#pragma once

#include <string>
#include <memory>
#include <functional>
#include "echo-transport.h"
#include "local-echo-transport.h"
#include "tcp-echo-transport.h"

using std::string;

enum TransportType {
	local_echo,
	tcp_echo
};

class Client {
public:
	/* Constructs a client. Arguments:
	 * - transporter: local_echo is meant to be run without a server, user
	 * input is echood back. 
	 * - portnr: the portnumber of the corresponding server, in case of
	 * TransportType tcp_echo
	 */
	Client(TransportType transporter=local_echo, uint16_t portnr=0);
	~Client();
	void send(const string& message);
	void print(const string& message);
	/* The following function should be called in main after constructing a 
   	 * client. It keeps accepting user input until the user types 'exit', 
   	 * after which the main loop of the function is exited.
   	 */
	void userInput();
private:
	TransportType type_;
	string name_;
	std::unique_ptr<EchoTransport> transporter_;
};