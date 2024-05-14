#include <iostream>
#include <cstdlib>
#include <cstring>
#include "boost_chat_client.hpp"

int main(int argc, char* argv[])
{
	boost::asio::io_context ioContext;

	std::cout << "Boost chat client ..." << std::endl;

	if (argc < 2)
	{
		std::cout << "need a argv (port)" << std::endl;
		return -1;
	}

	return 0;
}