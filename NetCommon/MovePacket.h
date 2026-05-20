#pragma once
#include "Packet.h"

class S2CMovePacket :
    public IPacket
{
public:
    
    void Parse(std::string InString)override;

    std::string ToString() override;
};

