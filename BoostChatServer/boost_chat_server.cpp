#include "boost_chat_server.hpp"

void chatRoom::JoinRoom(sptrParticipant member)
{
	users.insert(member);
	for (const chatFormat& msg : msgq)
	{
		member->Send(msg);
	}
}

void chatRoom::QuitRoom(sptrParticipant member)
{
	users.erase(member);
}

void chatRoom::MsgSend(const chatFormat& message)
{
	msgq.push_back(message);
	while (msgq.size() > MAX_MSG_NUM)
	{
		msgq.pop_front();
	}
	
	for (auto member : users)
	{
		member->Send(message);
	}
}

void chatSession::Start()
{
	room.JoinRoom(shared_from_this());
	readHeader();
}

void chatSession::Send(const chatFormat& msg)
{
	bool bProgress = !writeMsgs.empty();

	writeMsgs.push_back(msg);
	if (!bProgress)
	{
		msgWrite();
	}
}

void chatSession::readHeader()
{
	auto self(shared_from_this());
	boost::asio::async_read(sock,
		boost::asio::buffer(readMsg.data().header, HEADER_LENGTH), 
		[this, self](boost::system::error_code ec, std::size_t)
		{
			if (!ec && readMsg.decodeHeader())
			{
				readBody();
			}
			else
			{
				room.QuitRoom(shared_from_this());
			}
		} );
}

void chatSession::readBody()
{
	auto self(shared_from_this());
	boost::asio::async_read(sock,
		boost::asio::buffer(readMsg.data().body, MAX_PAYLOAD_LENGTH),
		[this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				room.MsgSend(readMsg);
				readHeader();
			}
			else
			{
				room.QuitRoom(shared_from_this());
			}
		});
}

void chatSession::msgWrite()
{
	auto self(shared_from_this());
	boost::asio::async_write(sock,
		boost::asio::buffer(writeMsgs.front().data().body, writeMsgs.front().length()),
		[this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				writeMsgs.pop_front();
				if (!writeMsgs.empty())
				{
					msgWrite();
				}
			}
			else
			{
				room.QuitRoom(shared_from_this());
			}
		});
}

void chatServer::doAccept()
{
	acceptor.async_accept(
		[this](boost::system::error_code ec, tcp::socket socket)
		{
			if (!ec)
			{
				std::make_shared<chatSession>(std::move(socket), room)->Start();
			}

			doAccept();
		});
}