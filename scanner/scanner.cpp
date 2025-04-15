#include "scanner.h"

namespace RoomScanner
{

Scanner::Scanner(ILidar& lidar, 
    IServo& azimuth_servo, 
    IServo& elevation_servo,
    std::chrono::milliseconds scan_interval, 
    float azimuth_interval,
    float elevation_interval
) : 
m_lidar(lidar), 
m_azimuth_servo(azimuth_servo), 
m_elevation_servo(elevation_servo),
m_scan_interval(scan_interval),
m_azimuth_interval(azimuth_interval),
m_elevation_interval(elevation_interval)
{
    CreateScanningPlan();
}

void Scanner::CreateScanningPlan()
{
    // Determine how many azimuth and elevation angles are required in the scan plan
    uint32_t azimuth_count = m_azimuth_servo.GetUpperLimitAngleDegrees() / m_azimuth_interval;
    uint32_t elevation_count = m_elevation_servo.GetUpperLimitAngleDegrees() / m_elevation_interval;
    uint64_t coordinate_count = azimuth_count * elevation_count;
    
    m_coordinates = std::vector<Coordinate>(coordinate_count);
    
    // Determine the azimuth and elevation angle values for each scan vector

    float az_angle = 0;
    float el_angle = 0;

    AngleScanDirection az_direction = AngleScanDirection::POSITIVE;

    // In short, these loops create a scanning plan that scans the entire azimuth range for each elevation angle, then repeats for the next elevation angle
    for(uint64_t el_index = 0; el_index < elevation_count; ++el_index)
    {
        for(uint64_t az_index = 0; az_index < azimuth_count; ++az_index)
        {
            // Create the coordinate
            m_coordinates[el_index + az_index] = Coordinate(az_angle,el_angle,0);

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
                az_angle += m_azimuth_interval;
            }
            else if(az_direction == AngleScanDirection::NEGATIVE)
            {
                az_angle -= m_azimuth_interval;
            }

            // Elevation always increments. Note that the elevation angle must start at zero.
            el_angle += m_elevation_interval;
        }
    }
}

bool Scanner::Run()
{
    for(auto it = m_coordinates.begin(); it != m_coordinates.end(); ++it)
    {
        // Save the distance coming from the lidar sensor
        it->distance_cm = static_cast<float>(m_lidar.GetDistance());
        // Wait for the servos to point to target angles
        sleep_ms(m_scan_interval.count());

        m_azimuth_servo.SetTargetAngle(it->azimuth_degrees);
        m_elevation_servo.SetTargetAngle(it->elevation_degrees);
        printf("Scanner::Run() -> az: {%f}, el: {%f}", it->azimuth_degrees, it->elevation_degrees);
    }

    return true;
}

}