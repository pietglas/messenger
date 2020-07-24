#include "local-echo-transport.h"

void LocalEchoTransport::send(const string& message) {
	sendClient(message);
}

void LocalEchoTransport::sendClient(const string& message) {
	pass_message_(message);
}

void LocalEchoTransport::setPassMessage(PassMessage msg) {
	pass_message_ = msg;
}

void LocalEchoTransport::disconnect() {}

