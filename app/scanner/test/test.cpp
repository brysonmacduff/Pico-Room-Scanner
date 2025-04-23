#include "scanner.h"
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include <deque>
#include <cmath>

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
        m_targeted_angles.push_back(degrees);
        return true;
    }

    float GetUpperLimitAngleDegrees() override
    {
        return UPPER_LIMIT_ANGLE_DEGREES;
    }

    auto BeginTargetedAngles() const
    {
        return m_targeted_angles.begin();
    }

    auto EndTargetedAngles() const
    {
        return m_targeted_angles.end();
    }

    size_t GetTargetedAngleCount() const
    {
        return m_targeted_angles.size();
    }

protected:
    static constexpr float UPPER_LIMIT_ANGLE_DEGREES = 180;

    std::deque<float> m_targeted_angles;
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

    /* This function checks the scanning sequence plan that the Scanner class creates when initialized. The following are checked:
        - The right number of angles have been targeted
        - The angular dispacement between successive angles is correct
        - The scan sweep direction is correct at all times for both az and el axes
    */
    static bool ValidateScanningPlan(const Scanner& scanner, 
        size_t expected_coordinate_count, 
        float azimuth_interval_degrees,
        float elevation_interval_degrees
    )
    {
        if(scanner.GetCoordinateCount() != expected_coordinate_count)
        {
            spdlog::error("ScannerTest::{}() -> Coordinate count is wrong. Expected: {}, Received: {}",__func__,expected_coordinate_count,scanner.GetCoordinateCount());
            return false;
        }

        bool is_az_adding = true;

        float expected_az = -azimuth_interval_degrees;
        float expected_el = -elevation_interval_degrees;

        // Simulate the expected angular scanning sweep by updating the expected azimuth and elevation according to a characteristic scanning sweep
        auto update_expected_angles = [&](Scanner::Coordinate coordinate) -> void
        {
            if(is_az_adding)
            {
                expected_az += azimuth_interval_degrees;
            }
            else
            {
                expected_az -= azimuth_interval_degrees;
            }

            // invert azimuth direction and increment elevation when an azimuth boundary is reached
            if(coordinate.azimuth_degrees <= 0)
            {
                is_az_adding = true;
                expected_el += elevation_interval_degrees;
            }
            else if(coordinate.azimuth_degrees >= scanner.GetAzimuthUpperLimitDegrees())
            {
                is_az_adding = false;
                expected_el += elevation_interval_degrees;
            }
        };

        for(auto it = scanner.BeginCoordinates(); it != scanner.EndCoordinates(); ++it)
        {
            update_expected_angles(*it);

            if(it->azimuth_degrees != expected_az)
            {
                spdlog::error("ScannerTest::{}() -> Azimuth angle is wrong. Expected: {}, Received: {}",__func__,expected_az,it->azimuth_degrees);
                return false;
            }
            else if(it->elevation_degrees != expected_el)
            {
                spdlog::error("ScannerTest::{}() -> Elevation angle is wrong. Expected: {}, Received: {}",__func__,expected_el,it->elevation_degrees);
                return false;
            }
        }

        return true;
    }
};

TEST_F(ScannerTest, Constructor)
{
    ASSERT_NO_FATAL_FAILURE(Scanner scanner(m_mock_lidar,
        m_mock_azimuth_servo,
        m_mock_elevation_servo,
        m_mock_sleeper,
        SCAN_INTERVAL,
        AZIMUTH_INTERVAL_DEGREES,
        ELEVATION_INTERVAL_DEGREES,
        false));
}

TEST_F(ScannerTest, ScanningPlan)
{
    Scanner scanner(m_mock_lidar,
        m_mock_azimuth_servo,
        m_mock_elevation_servo,
        m_mock_sleeper,
        SCAN_INTERVAL,
        AZIMUTH_INTERVAL_DEGREES,
        ELEVATION_INTERVAL_DEGREES,
        false
    );

    ASSERT_TRUE(scanner.Run());

    const size_t expected_angle_count = std::pow(m_mock_azimuth_servo.GetUpperLimitAngleDegrees() / AZIMUTH_INTERVAL_DEGREES, 2);
    
    const bool is_scan_sequence_valid = ValidateScanningPlan(scanner,expected_angle_count,AZIMUTH_INTERVAL_DEGREES,ELEVATION_INTERVAL_DEGREES);

    ASSERT_TRUE(is_scan_sequence_valid);
}

}
