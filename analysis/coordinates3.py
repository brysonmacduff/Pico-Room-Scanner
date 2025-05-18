
from enum import Enum
from coordinate3 import Coordinate3

import matplotlib.pyplot as plt

class Component(Enum):
    X = 0
    Y = 1
    Z = 2

class Coordinates3:
    
    def __init__(self):
        self.coordinates = list[Coordinate3]()

    def append(self, coordinate:Coordinate3):
        self.coordinates.append(coordinate)

    def get_components(self, component:Component) -> list[float]:
        components = list[float]()

        for coordinate in self.coordinates:

            match component:
                case Component.X:
                    components.append(coordinate.x)
                case Component.Y:
                    components.append(coordinate.y)
                case Component.Z:
                    components.append(coordinate.z)

        return components

    def generate_plot(self):
        # Sample data points
        x = self.get_components(Component.X)
        y = self.get_components(Component.Y)
        z = self.get_components(Component.Z)

        # Create a figure and 3D axis
        fig = plt.figure()
        ax = fig.add_subplot(111, projection='3d')

        # Plot the points
        ax.scatter(x, y, z, c='b', marker='o')

        # Labels and title
        ax.set_xlabel('X Axis')
        ax.set_ylabel('Y Axis')
        ax.set_zlabel('Z Axis')
        ax.set_title('3D Scan Plot')

        # Show the plot
        plt.show()

    def __str__(self) -> str:
        output:str = ""
        for coordinate in self.coordinates:
            output += coordinate.__str__() + "\n"
        return output

from coordinates3 import Coordinates3,Coordinate3

def test():
    coordinates:Coordinates3 = Coordinates3()
    coordinates.append(Coordinate3(1,1,1))
    coordinates.append(Coordinate3(2,2,2))
    coordinates.append(Coordinate3(3,3,3))
    coordinates.generate_plot()

if __name__ == "__main__":
    test()