#include "Client.h"

Client::Client()
	: mConnected(false) 
{

}

void Client::init()
{
	// Create socket
	std::cout << "Initialising socket... ";
	if ((mSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		throw std::runtime_error("Error while initialising the socket: " + WSAGetLastError());
	std::cout << "Success." << std::endl;

}

bool Client::makeConnection(const std::string& ip, unsigned int port)
{
	mConnection.sin_addr.s_addr = inet_addr(ip.c_str());
	mConnection.sin_family = AF_INET;
	mConnection.sin_port = htons(port);

	// Connect to remote server
	if (connect(mSocket, (struct sockaddr*)&mConnection, sizeof(mConnection)) < 0)
	{
		mConnected = false;
		return false;
	}

	mConnected = true;
	return true;
}

bool Client::sendMessage(char* message)
{
	if (!mConnected)
		std::cout << "No connection established, please connect first." << std::endl;

	if (send(mSocket, message, strlen(message), 0) < 0)
	{
		return false;
	}

	return true;
}

void Client::recvMessage()
{
	if (!mConnected)
		std::cout << "No connection established, please connect first." << std::endl;

	char buffer[512];
	int result = 0;

	result = recv(mSocket, buffer, sizeof(buffer),0);

	if (result > 0)
	{
		std::cout << buffer << std::endl;
	}
	else if (result == 0)
	{
		std::cout << "Connection closed." << std::endl;
		mConnected = false;
	}
}

Client::~Client()
{
	closesocket(mSocket);
}

