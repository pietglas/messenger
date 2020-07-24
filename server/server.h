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
#include <set>

using std::thread;

class Server {
public:
	/* Initializes a server object with portnumber portnr. Clients can connect
	 * to the server by providing their constructor with the same portnumber. 
	 */
	Server(uint16_t portnr);
	~Server();
	
	/* The following method should be called in the main function of the server
	 * program, after a server has been initialized. It waits for 20 seconds, 
	 * and exits if no client tried to connect in that period. If at some point
	 * all clients disconnect, the function will also return after a few seconds,
	 * at most 20. 
	 * For every client that connects, a seperate thread is created that waits
	 * for messages from this particular client. 
	 */
	void acceptClientCalls();
private:
	int list_socket_;
	sockaddr_in hint_;
	std::set<int> clients_;
	std::vector<thread> client_threads_;

	/* this method is set in a new thread when a client client_socket wants
	 * to connect.
	 * Exits when the client disconnects. 
	 */ 
	void receiveMessagesClient(int client_socket);
};