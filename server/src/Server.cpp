#include "Server.h"

void Server::init(unsigned int port)
{
	// Create socket
	std::cout << "Initialising socket... ";
	if ((mSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		throw std::runtime_error("Error while initialising the socket: " + WSAGetLastError());
	std::cout << "Success." << std::endl;

	// Init sockaddr_in structure
	mConnection.sin_family = AF_INET;
	mConnection.sin_addr.s_addr = INADDR_ANY;
	mConnection.sin_port = htons( port );

	// Bind
	std::cout << "Binding... ";
	if (bind(mSocket, (struct sockaddr*)& mConnection, sizeof(mConnection)) == SOCKET_ERROR)
		throw std::runtime_error("Error while binding: " + WSAGetLastError());
	std::cout << "Success." << std::endl;
}

int Server::sendMessage(SOCKET socket, const std::string& data)
{
	unsigned long dataSize = htonl(data.size());

	int result = sendMessage(socket, &dataSize, sizeof(dataSize));
	if (result == 1)
		result = sendMessage(socket, data.c_str(), data.size());

	return result;
}

int Server::sendMessage(SOCKET socket, const void* data, int dataSize)
{
	int bytesSend;
	const char* dataPtr = (const char*)data;

	while (dataSize > 0)
	{
		bytesSend = send(socket, dataPtr, dataSize,0);
		if (bytesSend == SOCKET_ERROR)
			return -1;

		dataPtr += bytesSend;
		dataSize -= bytesSend;
	}

	return 1;
}



void Server::recvMessages() 
{
	
}

int Server::recvMessage(SOCKET socket, void* data, int dataSize)
{
	char* dataPtr = (char*) data;

	int bytesRecv;

	while (dataSize > 0)
	{
		bytesRecv = recv(socket, dataPtr, dataSize, 0);
		if (bytesRecv <= 0)
			return bytesRecv;

		dataPtr += bytesRecv;
		dataSize -= bytesRecv;
	}

	return 1;
}

int Server::recvMessage(SOCKET socket, std::string& data)
{
	int result;
	unsigned long dataSize;

	data = "";
	result = recvMessage(socket, &dataSize, sizeof(dataSize));

	if (result == 1)
	{
		dataSize = ntohl(dataSize);

		if (dataSize > 0)
		{
			data.resize(dataSize);

			result = recvMessage(socket, &data[0],dataSize);
			if (result != 1)
				data.clear();
		}
	}

	return result;
}

std::string Server::getPeerName(Client client)
{
	std::string name = "";

	name += inet_ntoa(client.connection.sin_addr);
	name += ":";
	name += std::to_string(ntohs(client.connection.sin_port));
	return name;
}

void Server::run()
{
	// set of file descriptors(socket)
	fd_set readfds;

	// Sockets needed
	SOCKET tempSocket;
	SOCKET newSocket;

	int maxsd;

	struct sockaddr_in tempConnection; 
	int size = sizeof(struct sockaddr_in);

	// data buffer
	std::string data;

	// Set server socket as listening
	std::cout << "Listening... ";
	if (listen(mSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::runtime_error("Error while listening: " + WSAGetLastError());
	std::cout << "Ready." << std::endl;

	// Server loop
	while (true)
	{
		// Clear socket set
		FD_ZERO(&readfds);

		// Add server socket to set
		FD_SET(mSocket, &readfds);
		maxsd = mSocket;

		// Add client sockets to set
		for (int i = 0; i < mClients.size(); i++)
		{
			tempSocket = mClients[i]->socket; 

			FD_SET(tempSocket,&readfds);

			if (tempSocket > maxsd)
				maxsd = tempSocket;
		}
			// Wait for an activity on one of the sockets
			int activity = select(maxsd + 1, &readfds, NULL, NULL, NULL);

			if (activity < 0)
				throw std::runtime_error("Select Error.");

			// Master socket event = incoming connection
			if (FD_ISSET(mSocket, &readfds))
			{
				newSocket = accept(mSocket, (struct sockaddr*)&tempConnection, &size);
				if (newSocket < 0)
				{
					throw std::runtime_error("Cannot accept");
				}

				// Add client to vector 
				std::unique_ptr<Client> client(new Client);

				client->socket = newSocket;
				client->connection = tempConnection;
				client->name = getPeerName(*client);

				std::cout << "New connection: " << client->name << std::endl;

				mClients.push_back(std::move(client));
			}

			for (int i = 0; i < mClients.size(); i++)
			{
				tempSocket = mClients[i]->socket;

				if (FD_ISSET(tempSocket, &readfds))
				{
					// Someone disconnected
					int result = recvMessage(tempSocket, data);
					if (result <= 0)
					{
						std::cout << "User disconnected: " << mClients[i]->name << std::endl;
						// Close socket and remove from vector 
						closesocket(tempSocket);
						mClients.erase(mClients.begin() + i);
					}
					// Do something with message
					else
					{
						std::cout << "Message came in: " << data << std::endl; 
						for (int i = 0; i < mClients.size();i++)
							sendMessage(mClients[i]->socket,data);
					}
				}
			}	
	}
}

Server::~Server()
{
	closesocket(mSocket);
}