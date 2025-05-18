from coordinate3 import Coordinate3
from coordinates3 import Coordinates3

import re, os

class ScannerLogReader:

    # Matches values between curly braces
    SCAN_DATA_REGEX_PATTERN:str = r"\{(.*?)\}"

    def __init__(self, log_file_path:str):
        self.log_file_path = log_file_path

    def get_coordinates(self) -> Coordinates3:

        coordinates:Coordinates3 = Coordinates3()

        with open(self.log_file_path, "r") as file:
            for line in file:
                matches:list[str] = re.findall(self.SCAN_DATA_REGEX_PATTERN, line)

                if(len(matches) == 0):
                    continue
                
                azimuth_deg:float = float(matches[0])
                polar_deg:float = float(matches[1])
                range:float= float(matches[2].replace("cm",""))

                coordinates.append(Coordinate3.from_circular(range, azimuth_deg, polar_deg))

        return coordinates

def test():

    file_path = "sample.txt"

    # Open a file in write mock data
    with open(file_path, "w") as file:
        lines = [
            "main() -> Starting scanning sequence",
            "Scanner::Run() -> az: {0.000000}, el: {0.000000}, range: {83.000000 cm}\n",
            "Scanner::Run() -> az: {15.000000}, el: {60.000000}, range: {80.000000 cm}\n",
            "Scanner::Run() -> az: {30.000000}, el: {90.000000}, range: {93.000000 cm}\n"
        ]

        file.writelines(lines)

    slr:ScannerLogReader = ScannerLogReader(file_path)
    print(slr.get_coordinates())

    if os.path.exists(file_path):
        os.remove(file_path)

if __name__ == "__main__":
    test()