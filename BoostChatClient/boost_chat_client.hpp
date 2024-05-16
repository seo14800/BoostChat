#pragma once

#include <iostream>
#include <cstdlib>
#include <deque>
#include <thread>
#include <boost/asio.hpp>
#include "msg_format.hpp"

using boost::asio::ip::tcp;

typedef std::deque<chatFormat> msgQueue;

class chatClient
{
public:
	chatClient(boost::asio::io_context& ioContxt, const tcp::resolver::results_type& endpoints)
		:ioContext(ioContxt), sock(ioContxt)
	{
		doConnect(endpoints);
	}

	void Wrtie(const chatFormat& msg);
	void Close();

private:
	void doConnect(const tcp::resolver::results_type& endpoints);
	void readHeader();
	void readBody();
	void msgWrite();

	boost::asio::io_context& ioContext;
	tcp::socket sock;
	chatFormat readMsg;
	msgQueue writeMsgs;
};