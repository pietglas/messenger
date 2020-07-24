#pragma once

#include <functional>
#include <string>

using std::string;
typedef std::function<void(string)> PassMessage;

class EchoTransport {
public:
	virtual ~EchoTransport() {}
	virtual void send(const string& message) = 0;
	virtual void sendClient(const string& message) = 0;

	virtual void setPassMessage(PassMessage msg) = 0;
	virtual void disconnect() = 0;
};