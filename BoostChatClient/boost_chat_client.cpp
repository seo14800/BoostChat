#include "boost_chat_client.hpp"

void chatClient::Wrtie(const chatFormat& msg)
{
	boost::asio::post(ioContext,
		[this, msg]()
		{
			bool bProgress = !writeMsgs.empty();
			writeMsgs.push_back(msg);
			if (!bProgress)
			{
				msgWrite();
			}
		});
}

void chatClient::Close()
{
	boost::asio::post(ioContext, [this]() {	sock.close(); });
}

void chatClient::doConnect(const tcp::resolver::results_type& endpoints)
{
	boost::asio::async_connect(sock, endpoints,
		[this](boost::system::error_code ec, tcp::endpoint)
		{
			if (!ec)
			{
				readHeader();
			}
		});
}

void chatClient::readHeader()
{
	boost::asio::async_read(sock, 
		boost::asio::buffer(readMsg.data().header, HEADER_LENGTH),
		[this](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec && readMsg.decodeHeader())
			{
				readBody();
			}
			else
			{
				sock.close();
			}
		});
}

void chatClient::readBody()
{
	boost::asio::async_read(sock, 
		boost::asio::buffer(readMsg.data().body, MAX_PAYLOAD_LENGTH),
		[this](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				std::cout.write(readMsg.getBody(), MAX_PAYLOAD_LENGTH) << std::endl;
				readHeader();
			}
			else
			{
				sock.close();
			}
		});
}

void chatClient::msgWrite()
{
	boost::asio::async_write(sock,
		boost::asio::buffer(writeMsgs.front().data().body, writeMsgs.front().length()),
		[this](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				writeMsgs.pop_front();
				if (!writeMsgs.empty())
				{
					msgWrite();
				}
				else
				{
					sock.close();
				}
			}
		});
}