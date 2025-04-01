namespace RoomScanner
{
class IServo
{
public:
    virtual ~IServo() = default;
    // Set the angle that the servo will point to
    virtual bool SetTargetAngle(float degrees) = 0;
protected:
    // force inheriting classes to set an upper limit
    IServo(float upper_limit_degrees){}
};
}