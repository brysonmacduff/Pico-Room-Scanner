#include "i_sleeper.h"
#include <pico/stdlib.h>

namespace RoomScanner
{
class PicoSleeper : public ISleeper
{
public:
    PicoSleeper() = default;
    ~PicoSleeper() = default;
    void SleepMilliseconds(uint32_t duration_ms) override;
};
} // namespace RoomScanner