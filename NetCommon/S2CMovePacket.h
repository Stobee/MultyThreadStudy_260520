#pragma once
#include "Packet.h"
class S2CMovePacket :
    public IPacket
{
public:

    unsigned short UserNum;
    short UserX;
    short UserY;

    void Parse(std::string InString)override;

    std::string ToString() override;

    int Length() override;
};

