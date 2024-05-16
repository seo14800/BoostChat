#include "boost_chat_server.hpp"

int main(int argc, char *argv[])
{
	boost::asio::io_context ioContext;
	std::list<chatServer> vServers;

	try
	{
		std::cout << "Boost chat server ..." << std::endl;

		if (argc < 2)
		{
			std::cout << "need arguments (ports)" << std::endl;
			return -1;
		}

		for (int i = 1; i < argc; i++)
		{
			std::cout << "port : " << argv[i] << std::endl;
			tcp::endpoint ep(tcp::v4(), std::atoi(argv[i]));
			vServers.emplace_back(ioContext, ep);
		}

		ioContext.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception : " << e.what() << "\n";
	}

	return 0;
}