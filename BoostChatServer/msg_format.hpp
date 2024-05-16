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
    char header[HEADER_LENGTH];
    char body[MAX_PAYLOAD_LENGTH];
};

class chatFormat
{
public:
    chatFormat()
        : bodyLength(0)
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
        return HEADER_LENGTH + bodyLength;
    }

    const char* getBody() const
    {
        return msg.body;
    }

    void setBody(char* cpLine, size_t len)
    {
        std::memcpy(msg.body, cpLine, len);
    }

    std::size_t getBodyLength() const
    {
        return bodyLength;
    }

    void setBodyLength(std::size_t new_length)
    {
        bodyLength = new_length;
        if (bodyLength > MAX_PAYLOAD_LENGTH)
            bodyLength = MAX_PAYLOAD_LENGTH;
    }

    bool decodeHeader()
    {
        char caHeader[HEADER_LENGTH + 1] = "";
        std::strncat(caHeader, msg.header, HEADER_LENGTH);
        bodyLength = std::atoi(caHeader);
        //bodyLength = msg.header.msgLength;
        if (bodyLength > MAX_PAYLOAD_LENGTH)
        {
            bodyLength = 0;
            return false;
        }
        return true;
    }

    void endcodeHeader()
    {
        chatHeader header;
        header.msgLength = static_cast<unsigned int>(bodyLength);
        std::memcpy(&msg.header, &header, HEADER_LENGTH);
    }

private:
    chatMsg msg;
    std::size_t bodyLength;
};