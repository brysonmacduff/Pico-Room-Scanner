#pragma once
#include <cstdint>

namespace RoomScanner
{
class ILidar
{
public:
    ~ILidar() = default;
    virtual uint32_t GetDistance() = 0;
};
}