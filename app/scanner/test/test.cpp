#include "scanner.h"
#include <gtest/gtest.h>

using namespace RoomScanner;

namespace RoomScanner::Test
{

class MockLidar : public ILidar
{
public:
    MockLidar() = default;
    ~MockLidar() = default;

    uint32_t GetDistance() override
    {
        return 0;
    }
};

class MockServo : public IServo
{
public:
    MockServo() = default;  
    ~MockServo() = default;

    bool SetTargetAngle(float degrees) override
    {
        return true;
    }

    float GetUpperLimitAngleDegrees() override
    {
        return UPPER_LIMIT_ANGLE_DEGREES;
    }

protected:
    static constexpr float UPPER_LIMIT_ANGLE_DEGREES = 180;
};

class MockSleeper : public ISleeper
{
public:
    MockSleeper() = default;
    ~MockSleeper() = default;

    void SleepMilliseconds(uint32_t duration_ms) override
    {
        return;
    }
};

class ScannerTest : public ::testing::Test
{
protected:
    void TearDown() override {}
    void SetUp() override {}

    static constexpr std::chrono::milliseconds SCAN_INTERVAL {1000};
    static constexpr float AZIMUTH_INTERVAL_DEGREES = 15;
    static constexpr float ELEVATION_INTERVAL_DEGREES = 15;

    MockServo m_mock_azimuth_servo;
    MockServo m_mock_elevation_servo;
    MockLidar m_mock_lidar;
    MockSleeper m_mock_sleeper;
};

TEST_F(ScannerTest, Constructor)
{
    ASSERT_NO_FATAL_FAILURE(Scanner scanner(m_mock_lidar,
        m_mock_azimuth_servo,
        m_mock_elevation_servo,
        m_mock_sleeper,
        SCAN_INTERVAL,
        AZIMUTH_INTERVAL_DEGREES,
        ELEVATION_INTERVAL_DEGREES));
}
}
