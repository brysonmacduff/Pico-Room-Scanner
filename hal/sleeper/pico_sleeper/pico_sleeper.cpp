#include "pico_sleeper.h"

namespace RoomScanner
{
void PicoSleeper::SleepMilliseconds(uint32_t duration_ms)
{
    sleep_ms(duration_ms);
}
} // RoomScanner