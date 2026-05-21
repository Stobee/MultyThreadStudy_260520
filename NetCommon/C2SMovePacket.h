#pragma once
#include "Packet.h"

class C2SMovePacket :
    public IPacket
{
public:
    
    char MovementInput = 0;

    void Parse(std::string InString)override;

    std::string ToString() override;

    int Length() override;
};

