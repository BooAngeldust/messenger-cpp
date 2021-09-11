#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <winsock2.h>
#include <vector>
#include <memory>

#include <iostream>
#include <sys/time.h>

class Server
{
private:
	struct Client
	{
		SOCKET socket;
		struct sockaddr_in connection;
		std::string name;
	};

private:
	SOCKET mSocket;
	struct sockaddr_in mConnection;
	std::vector<std::unique_ptr<Client>> mClients;

private:
	void recvMessages();

	int sendMessage(SOCKET socket, const std::string& message);
	int sendMessage(SOCKET socket, const void* data, int dataSize);

	int recvMessage(SOCKET socket, void* data, int dataSize);
	int recvMessage(SOCKET socket, std::string& data);

	std::string getPeerName(Client client);

public:
	void init(unsigned int port);

	void run();

	~Server();
};

#endif
