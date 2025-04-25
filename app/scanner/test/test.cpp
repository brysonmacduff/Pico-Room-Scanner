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

    float GetUpperLimitAngleDegrees() const override
    {
        return UPPER_LIMIT_ANGLE_DEGREES;
    }

    std::deque<float>::const_iterator BeginTargetedAngles() const
    {
        return m_targeted_angles.begin();
    }

    std::deque<float>::const_iterator EndTargetedAngles() const
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

    /* This function validates that the scanning plan was correct dispatched to the azimuth servo by checking the following:
        - Validates that azimuth angles are changing in the right direction (up or down)
        - Validates that the azimuth angles are changing at the right angular interval
        - Validates that the correct number of azimuth angles have been received
    */
    static bool ValidateTargetedAngles(const MockServo& mock_az_servo, const MockServo& mock_el_servo, float azimuth_interval_degrees, float elevation_interval_degrees, int expected_angle_count)
    {
        if(mock_az_servo.GetTargetedAngleCount() != expected_angle_count)
        {
            spdlog::error("ScannerTest::{}() -> Wrong number of azimuth angles. Expected: {}, Actual: {}",__func__,expected_angle_count,mock_az_servo.GetTargetedAngleCount());
            return false;
        }

        if(mock_el_servo.GetTargetedAngleCount() != expected_angle_count)
        {
            spdlog::error("ScannerTest::{}() -> Wrong number of elevation angles. Expected: {}, Actual: {}",__func__,expected_angle_count,mock_el_servo.GetTargetedAngleCount());
            return false;
        }

        bool is_azimuth_adding = true;
        float az_angle = 0;
        float el_angle = -elevation_interval_degrees;

        auto it_el = mock_el_servo.BeginTargetedAngles();
        auto it_az = mock_az_servo.BeginTargetedAngles();

        while(true)
        {
            if(it_az == mock_az_servo.EndTargetedAngles() || it_el == mock_el_servo.EndTargetedAngles())
            {
                break;
            }

            if(az_angle <= 0)
            {
                is_azimuth_adding = true;
                el_angle += elevation_interval_degrees;
            }
            else if(az_angle >= mock_az_servo.GetUpperLimitAngleDegrees())
            {
                is_azimuth_adding = false;
                el_angle += elevation_interval_degrees;
            }

            if(az_angle != *it_az)
            {
                spdlog::error("ScannerTest::{}() -> Wrong azimuth angle. Expected: {}, Actual: {}",__func__, az_angle, *it_az);
                return false;
            }
            
            if(el_angle != *it_el)
            {
                spdlog::error("ScannerTest::{}() -> Wrong elevation angle. Expected: {}, Actual: {}",__func__, el_angle, *it_el);
                return false;
            }

            if(is_azimuth_adding)
            {
                az_angle += azimuth_interval_degrees;
            }
            else
            {
                az_angle -= azimuth_interval_degrees;
            }

            ++it_az;
            ++it_el;
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

// Verify that the scanning plan is valid
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

    const size_t expected_angle_count = std::pow(m_mock_azimuth_servo.GetUpperLimitAngleDegrees() / AZIMUTH_INTERVAL_DEGREES, 2);
    
    const bool is_scan_sequence_valid = ValidateScanningPlan(scanner,expected_angle_count,AZIMUTH_INTERVAL_DEGREES,ELEVATION_INTERVAL_DEGREES);

    ASSERT_TRUE(is_scan_sequence_valid);
}

// Verify the targeted azimuth and elevation angles are correct.
TEST_F(ScannerTest, Run)
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

    const bool is_valid = ValidateTargetedAngles(m_mock_azimuth_servo,m_mock_elevation_servo,AZIMUTH_INTERVAL_DEGREES,ELEVATION_INTERVAL_DEGREES,expected_angle_count);

    ASSERT_TRUE(is_valid);
}

}
