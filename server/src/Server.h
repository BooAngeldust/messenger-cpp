#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <winsock2.h>
#include <vector>
#include <memory>

struct Client
{
	SOCKET socket;
	struct sockaddr_in connection;
	std::string name;
};

class Server
{
private:
	SOCKET mSocket;
	struct sockaddr_in mConnection;
	std::vector<std::unique_ptr<Client>> mClients;

private:
	void acceptConnections();
	void recvMessages();

	void sendMessage(SOCKET socket, const std::string& message);

public:
	void init(unsigned int port);

	void run();

	~Server();
};

#endif
