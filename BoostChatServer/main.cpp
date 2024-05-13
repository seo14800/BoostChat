#include <iostream>
#include <cstdlib>
#include <cstring>
#include "chat_server.hpp"

int main(int argc, char *argv[])
{
	boost::asio::io_context ioContext;

	std::cout << "Boost chat server ..." << std::endl;

	if (argc < 2)
	{
		std::cout << "need a argv (port)" << std::endl;
		return -1;
	}


	return 0;
}