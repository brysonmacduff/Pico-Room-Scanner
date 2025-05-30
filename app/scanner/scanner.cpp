#include "scanner.h"

namespace RoomScanner
{

Scanner::Scanner(ILidar& lidar, 
    IServo& azimuth_servo, 
    IServo& elevation_servo,
    ISleeper& sleeper,
    std::chrono::milliseconds scan_interval, 
    float azimuth_interval,
    float elevation_interval,
    bool is_verbose
) : 
m_lidar(lidar), 
m_azimuth_servo(azimuth_servo), 
m_elevation_servo(elevation_servo),
m_sleeper(sleeper),
m_scan_interval(scan_interval),
m_azimuth_interval_degrees(azimuth_interval),
m_elevation_interval_degrees(elevation_interval),
m_is_verbose(is_verbose)
{
    CreateScanningPlan();
}

void Scanner::CreateScanningPlan()
{
    // Determine how many azimuth and elevation angles are required in the scan plan
    uint32_t azimuth_count = m_azimuth_servo.GetUpperLimitAngleDegrees() / m_azimuth_interval_degrees;
    uint32_t elevation_count = m_elevation_servo.GetUpperLimitAngleDegrees() / m_elevation_interval_degrees;
    uint64_t coordinate_count = azimuth_count * elevation_count;
    
    m_coordinates = std::vector<Coordinate>(coordinate_count);
    
    // Determine the azimuth and elevation angle values for each scan vector

    float az_angle = 0;
    float el_angle = 0;

    uint64_t coordinate_index = 0;

    AngleScanDirection az_direction = AngleScanDirection::POSITIVE;

    // In short, these loops create a scanning plan that scans the entire azimuth range for each elevation angle, then repeats for the next elevation angle
    for(uint64_t el_index = 0; el_index < elevation_count; ++el_index)
    {
        for(uint64_t az_index = 0; az_index < azimuth_count; ++az_index)
        {
            // Create the coordinate
            m_coordinates[coordinate_index] = Coordinate(az_angle,el_angle,0);
            ++coordinate_index;

            // Determine which way to rotate the azimuth servo
            if(az_angle >= m_azimuth_servo.GetUpperLimitAngleDegrees())
            {
                // Decrement azimuth angle back down from the upper limit when the angle limit is reached
                az_direction = AngleScanDirection::NEGATIVE;
            }
            else if(az_angle <= 0)
            {
                // Increment the azimuth angle up until the angular limit is reached
                az_direction = AngleScanDirection::POSITIVE;
            }
            
            if(az_direction == AngleScanDirection::POSITIVE)
            {
                az_angle += m_azimuth_interval_degrees;
            }
            else if(az_direction == AngleScanDirection::NEGATIVE)
            {
                az_angle -= m_azimuth_interval_degrees;
            }

        }

        // Elevation always increments. Note that the elevation angle must start at zero.
        el_angle += m_elevation_interval_degrees;
    }
}

bool Scanner::Run()
{
    for(auto it = m_coordinates.begin(); it != m_coordinates.end(); ++it)
    {
        m_azimuth_servo.SetTargetAngle(it->azimuth_degrees);
        m_elevation_servo.SetTargetAngle(it->elevation_degrees);

        // Wait for the servos to point to target angles
        m_sleeper.SleepMilliseconds(static_cast<uint32_t>(m_scan_interval.count()));

        // Save the distance coming from the lidar sensor
        it->distance_cm = static_cast<float>(m_lidar.GetDistance());

        if(m_is_verbose)
        {
            printf("Scanner::Run() -> az: {%f}, el: {%f}, range: {%f cm}\n", it->azimuth_degrees, it->elevation_degrees, it->distance_cm);
        }
    }

    return true;
}

std::vector<Scanner::Coordinate>::const_iterator Scanner::BeginCoordinates() const
{
    return m_coordinates.begin();
}

std::vector<Scanner::Coordinate>::const_iterator Scanner::EndCoordinates() const
{
    return m_coordinates.end();
}

size_t Scanner::GetCoordinateCount() const
{
    return m_coordinates.size();
}

float Scanner::GetAzimuthUpperLimitDegrees() const
{
    return m_azimuth_servo.GetUpperLimitAngleDegrees();
}

float Scanner::GetElevationUpperLimitDegrees() const
{
    return m_elevation_servo.GetUpperLimitAngleDegrees();
}

} // namespace RoomScanner