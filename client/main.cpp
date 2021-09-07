#include <winsock2.h>
#include <iostream>
#include <string>

#include "src/Client.h"

int main(int argc , char *argv[])
{
	WSADATA wsa;

	// Initialise Winsock
	std::cout << "Initialising Winsock... ";
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		std::cout << "Failed. Error Code : " << WSAGetLastError();
		return 1;
	}
	std::cout << "Success." << std::endl;

	// Create Client Instance
	Client client;
	client.init();

	// Connect to the server
	bool connection = client.makeConnection("127.0.0.1",8889);
	if (!connection)
	{
		std::cout << "No connection established with the server.";
		return 1;
	}
	
	client.run();

	WSACleanup();

	return 0;
}