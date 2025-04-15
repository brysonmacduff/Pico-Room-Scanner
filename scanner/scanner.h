#pragma once
#include "lidar_tf_luna.h"
#include "servo_miuzei_fs08md.h"
#include <vector>
#include <chrono>

namespace RoomScanner
{
class Scanner
{
public:

    struct Coordinate
    {
        float azimuth_degrees;
        float elevation_degrees;
        float distance_cm;

        ~Coordinate() = default;
        Coordinate() = default;

        Coordinate(float azimuth_degrees, float elevation_degrees, float distance_cm)
        {
            this->azimuth_degrees = azimuth_degrees;
            this->elevation_degrees = elevation_degrees;
            this->distance_cm = distance_cm;
        }

        Coordinate(const Coordinate& other)
        {
            azimuth_degrees = other.azimuth_degrees;
            elevation_degrees = other.elevation_degrees;
            distance_cm = other.distance_cm;
        }

        Coordinate& operator=(const Coordinate& other)
        {
            if(this == &other)
            {
                return *this;
            }

            azimuth_degrees = other.azimuth_degrees;
            elevation_degrees = other.elevation_degrees;
            distance_cm = other.distance_cm;
        }
    };

    Scanner(ILidar& lidar, 
        IServo& azimuth_servo, 
        IServo& elevation_servo, 
        std::chrono::milliseconds scan_interval, 
        float azimuth_interval,
        float elevation_interval
    );

    // Starts the scan sequence. Returns true once complete or false if scanning failed.
    bool Run();

protected:

    enum class AngleScanDirection
    {
        POSITIVE,
        NEGATIVE
    };

    void CreateScanningPlan();

    ILidar& m_lidar;
    IServo& m_azimuth_servo;
    IServo& m_elevation_servo;
    std::chrono::milliseconds m_scan_interval;
    float m_azimuth_interval;
    float m_elevation_interval;
  
    std::vector<Coordinate> m_coordinates;
};
}