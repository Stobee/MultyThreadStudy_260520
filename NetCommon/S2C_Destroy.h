#pragma once
#include "Packet.h"
class S2C_Destroy :
    public IPacket
{public:
    void Parse(std::string InString)override;

    std::string ToString() override;
};

