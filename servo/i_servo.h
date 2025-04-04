namespace RoomScanner
{
class IServo
{
public:
    virtual ~IServo() = default;
    // Set the angle that the servo will point to
    virtual bool SetTargetAngle(float degrees) = 0;
};
}