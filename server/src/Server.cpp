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

void Server::sendMessage(SOCKET socket, const std::string& message)
{
	send(socket , message.c_str() , strlen(message.c_str()) + 1 , 0);
}

void Server::acceptConnections()
{
	int counter = 0;

	std::cout << "Listening... ";
	if (listen(mSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::runtime_error("Error while listening: " + WSAGetLastError());
	std::cout << "Ready." << std::endl;

	int size = sizeof(struct sockaddr_in);

	while(true)
	{
		SOCKET new_socket;
		struct sockaddr_in connection; 

		new_socket = accept(mSocket, (struct sockaddr *)&connection,&size);

		if (new_socket != INVALID_SOCKET)
		{
			// If a connection is made push client to vector


			std::unique_ptr<Client> client(new Client);
			
			client->socket = new_socket;
			client->connection = connection;
			client->name = std::to_string(counter);

			std::cout << "New connection: " << client->name << std::endl;
			sendMessage(client->socket, "Welcome: " + client->name);

			mClients.push_back(std::move(client));
			counter++;
		}
	}
}

void Server::recvMessages()
{

}

void Server::run()
{
	acceptConnections();
}

Server::~Server()
{
	closesocket(mSocket);
}