#pragma once
#include "Packet.h"
class S2C_Login :
    public IPacket
{public:

    std::string Message;

    void Parse(std::string InString)override;

    std::string ToString() override;
};

