#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>
#include <iostream>

class Client
{
private:
	SOCKET mSocket;
	struct sockaddr_in mConnection;

	bool mConnected;

public:
	Client();

	void init();

	bool makeConnection(const std::string& ip, unsigned int port);
	bool sendMessage(char* message);

	int recvMessage(void* data, int dataSize);
	int recvMessage(std::string& data);

	int sendMessage(const std::string& message);
	int sendMessage(const void* data, int dataSize);


	~Client();
};


#endif