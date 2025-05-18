import sys

from scanner_log_reader import ScannerLogReader,Coordinates3

def main():
    if len(sys.argv) == 1:
        print("Error. Missing scan log file path.")
        return

    log_file_path:str = sys.argv[1]

    slr:ScannerLogReader = ScannerLogReader(log_file_path)
    coordinates:Coordinates3 = slr.get_coordinates()
    coordinates.generate_plot()

if __name__ == "__main__":
    main()