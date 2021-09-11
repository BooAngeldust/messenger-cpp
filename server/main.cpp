#include<winsock2.h>

#include "src/Server.h"

#define PORT 8889

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

	// Create server instance
	Server server;

	server.init(PORT);
	server.run();
	
	


	return 0;
}