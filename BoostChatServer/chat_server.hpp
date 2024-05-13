#pragma once

#include <iostream>
#include <cstdlib>
#include <boost/asio.hpp>
#include "chat_message.hpp"

using boost::asio::ip::tcp;
class participant
{
public:
	virtual ~participant() {}
	virtual void send() = 0;
};

typedef std::shared_ptr<participant> sptr_participant;

class room
{
private:

public:
	void join(sptr_participant member);
	void leave(sptr_participant member);
	void deliver(const chat_message& message);
};