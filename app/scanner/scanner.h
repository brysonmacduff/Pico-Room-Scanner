#pragma once

#include "i_servo.h"
#include "i_lidar.h"
#include "i_sleeper.h"

#include <vector>
#include <chrono>
#include <cstdio>

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

            return *this;
        }

    };

    Scanner(ILidar& lidar, 
        IServo& azimuth_servo, 
        IServo& elevation_servo,
        ISleeper& sleeper, 
        std::chrono::milliseconds scan_interval, 
        float azimuth_interval_degrees,
        float elevation_interval_degrees
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
    ISleeper& m_sleeper;
    std::chrono::milliseconds m_scan_interval;
    float m_azimuth_interval_degrees;
    float m_elevation_interval_degrees;
  
    std::vector<Coordinate> m_coordinates;
};
}