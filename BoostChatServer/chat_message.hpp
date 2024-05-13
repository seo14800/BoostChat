#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>

#define HEADER_LENGTH       24      // length(int) + reserved(20)
#define MAX_PAYLOAD_LENGTH  512

typedef struct chatHeader
{
    unsigned int msgLength;
    unsigned char reserved[20];
};

typedef struct chatMsg
{
    chatHeader header;
    char body[MAX_PAYLOAD_LENGTH];
};

class chat_message
{
private:
    chatMsg msg;
    std::size_t body_length;

public:
    chat_message()
        : body_length(0)
    {
    }

    const chatMsg data() const
    {
        return msg;
    }

    chatMsg data()
    {
        return msg;
    }

    std::size_t length() const
    {
        return HEADER_LENGTH + body_length;
    }

    const char* body() const
    {
        return msg.body;
    }

    char* body()
    {
        return msg.body;
    }

    std::size_t body_length() const
    {
        return body_length;
    }

    void body_length(std::size_t new_length)
    {
        body_length = new_length;
        if (body_length > MAX_PAYLOAD_LENGTH)
            body_length = MAX_PAYLOAD_LENGTH;
    }

    bool decode_header()
    {
        body_length = msg.header.msgLength;
        if (body_length > MAX_PAYLOAD_LENGTH)
        {
            body_length = 0;
            return false;
        }
        return true;
    }

    void encode_header()
    {
        chatHeader header;
        header.msgLength = static_cast<unsigned int>(body_length);
        std::memcpy(&msg.header, &header, HEADER_LENGTH);
    }
};