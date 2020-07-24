#pragma once

#include "echo-transport.h"
#include <functional>
#include <string>

class LocalEchoTransport : public EchoTransport {
public:
	void send(const string& message) override;
	void sendClient(const string& message) override;

	void setPassMessage(PassMessage msg) override;
	void disconnect() override;
private:
	PassMessage pass_message_;
};