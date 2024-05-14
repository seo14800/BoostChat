#pragma once

#include <iostream>
#include <cstdlib>
#include <deque>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>
#include "msg_format.hpp"

#define MAX_MSG_NUM		128

using boost::asio::ip::tcp;
class IParticipant
{
public:
	virtual ~IParticipant() {}
	virtual void Send(const chatFormat& msg) = 0;
};

typedef std::shared_ptr<IParticipant> sptrParticipant;
typedef std::deque<chatFormat> msgQueue;

class chatRoom
{
public:
	void JoinRoom(sptrParticipant member);
	void QuitRoom(sptrParticipant member);
	void MsgSend(const chatFormat& message);

private:
	msgQueue msgq;
	std::set<sptrParticipant> users;
};

class chatSession : public IParticipant, public std::enable_shared_from_this<chatSession>
{
public:
	chatSession(tcp::socket sessionSock, chatRoom& sessionRoom)
		: sock(std::move(sessionSock)), room(sessionRoom)
	{
	}

	void Start();
	void Send(const chatFormat& msg);

private:
	void readHeader();
	void readBody();
	void msgWrite();

	tcp::socket sock;
	chatRoom& room;
	chatFormat readMsg;
	msgQueue writeMsgs;
};

class chatServer
{
public:
	chatServer(boost::asio::io_context& ioContext, tcp::endpoint& endpoint)
		: acceptor(ioContext, endpoint)
	{
		doAccept();
	}
private:
	void doAccept();
	
	tcp::acceptor acceptor;
	chatRoom room;
};