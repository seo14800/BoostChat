#include "boost_chat_client.hpp"

int main(int argc, char* argv[])
{
	boost::asio::io_context ioContext;

	try
	{
		std::cout << "Boost chat client ..." << std::endl;

		if (argc < 2)
		{
			std::cout << "need a argv (port)" << std::endl;
			return -1;
		}

		tcp::resolver resolver(ioContext);
		auto endpoints = resolver.resolve(argv[1], argv[2]);
		chatClient c(ioContext, endpoints);

		std::thread t([&ioContext]() {ioContext.run(); });

		char caLine[MAX_PAYLOAD_LENGTH + 1];
		while (std::cin.getline(caLine, MAX_PAYLOAD_LENGTH + 1))
		{
			chatFormat msg;
			msg.setBodyLength(std::strlen(caLine));
			msg.setBody(caLine, std::strlen(caLine));
			msg.endcodeHeader();
			c.Wrtie(msg);
		}

		c.Close();
		t.join();
	}
	catch (std::exception& e)
	{
		std::cerr << "exception : " << e.what() << std::endl;
	}

	return 0;
}