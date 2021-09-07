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

int Client::recvMessage(void* data, int dataSize)
{
	char* dataPtr = (char*) data;

	int bytesRecv;

	while (dataSize > 0)
	{
		bytesRecv = recv(mSocket, dataPtr, dataSize, 0);
		if (bytesRecv <= 0)
			return bytesRecv;

		dataPtr += bytesRecv;
		dataSize -= bytesRecv;
	}

	return 1;
}

int Client::recvMessage(std::string& data)
{
	if (!mConnected)
		std::cout << "No connection established, please connect first." << std::endl;

	int result;
	unsigned long dataSize;

	data = "";
	result = recvMessage(&dataSize, sizeof(dataSize));

	if (result == 1)
	{
		dataSize = ntohl(dataSize);

		if (dataSize > 0)
		{
			data.resize(dataSize);

			result = recvMessage(&data[0],dataSize);
			if (result != 1)
				data.clear();
		}
	}

	return result;
}

int Client::sendMessage(const std::string& data)
{
	unsigned long dataSize = htonl(data.size());

	int result = sendMessage(mSocket, &dataSize, sizeof(dataSize));
	if (result == 1)
		result = sendMessage(mSocket, data.c_str(), data.size());

	return result;
}

int Client::sendMessage(const void* data, int dataSize)
{
	int bytesSend;
	const char* dataPtr = (const char*)data;

	while (dataSize > 0)
	{
		bytesSend = send(mSocket, dataPtr, dataSize,0);
		if (bytesSend == SOCKET_ERROR)
			return -1;

		dataPtr += bytesSend;
		dataSize -= bytesSend;
	}

	return 1;
}


Client::~Client()
{
	closesocket(mSocket);
}

