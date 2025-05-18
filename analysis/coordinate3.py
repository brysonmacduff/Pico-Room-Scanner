from math import sin,cos,radians

# Defines a cartesian coordinate in 3-space
class Coordinate3:
    def __init__(self, x:float, y:float, z:float):
        self.x = x
        self.y = y
        self.z = z

    # Creates a cartesian coordinate from spherical coordinates
    @staticmethod
    def from_circular(range:float, azimuth_deg:float, polar_deg:float):

        if(range < 0):
            raise Exception("Range value is less than zero.")
        
        azimuth_rad = radians(azimuth_deg)
        polar_rad = radians(polar_deg)

        x:float = range * sin(polar_rad) * cos(azimuth_rad)
        y:float = range * sin(polar_rad) * sin(azimuth_rad)
        z:float = range * cos(polar_rad)
        return Coordinate3(x,y,z)
    
    def __str__(self) -> str:
        return "["+str(self.x)+","+str(self.y)+","+str(self.z)+"]"