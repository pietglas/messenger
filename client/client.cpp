#include "client.h"
#include <iostream>
#include <chrono>
#include <ctime>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::getline;

Client::Client(TransportType transporter, uint16_t portnr) {
	// set the name of the client
	cout << "Enter your name: " << endl;
	getline(cin, name_);
	// set the transport type 
	switch(transporter) {
		case (tcp_echo):
			type_ = tcp_echo;
			transporter_ = std::make_unique<TCPEchoTransport>(portnr);
			break;
		default:	// local_echo
			type_ = local_echo;
			transporter_ = std::make_unique<LocalEchoTransport>();
	}
	transporter_->setPassMessage(std::bind(&Client::print, 
						this, std::placeholders::_1));
	cout << "Hello, " << name_ << "! You can now enter messages :)" 
		<< endl;
	cout << "If you want to exit the program, type 'exit'." << endl;
}

Client::~Client() {
	cerr << "destructor client called" << endl;
}

void Client::send(const string& message) {
	transporter_->send(message);
}

void Client::print(const string& message) {
	cout << message << endl;
}

void Client::userInput() {
	string message;
	string message_userized;
	while (1) {
		// wait for user input. Exit if user types 'exit' 
		getline(cin, message);
		if (message == string("exit"))
			break;
		// add user name and time at which the message was entered
		auto now = std::chrono::system_clock::now();
		std::time_t time = std::chrono::system_clock::to_time_t(now);
		message_userized = name_ + " wrote at " +
		string(std::ctime(&time)) + ": " + message;
		// send message to the server
		send(message_userized);
	}
	// disconnect from the server and exit
	transporter_->disconnect();
}