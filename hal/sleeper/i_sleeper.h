#pragma once
#include <cstdint>

namespace RoomScanner
{
class ISleeper
{
public:
    ~ISleeper() = default;
    virtual void SleepMilliseconds(uint32_t duration_ms) = 0;
};
}