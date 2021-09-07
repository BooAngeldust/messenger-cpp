#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <winsock2.h>
#include <vector>
#include <memory>



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
	void acceptConnections();
	void recvMessages();


	int sendMessage(SOCKET socket, const std::string& message);
	int sendMessage(SOCKET socket, const void* data, int dataSize);

	int recvMessage(void* data, int dataSize);
	int recvMessage(std::string& data);

public:
	void init(unsigned int port);

	void run();

	~Server();
};

#endif
