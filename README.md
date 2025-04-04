* Description
    - This is a project that builds a program for the Raspberry Pi Pico W for mapping the interior of rooms.
* Supported Physical Hardware
    - Note that this project is designed to use specific hardware devices. Alternative or equivalent hardware devices are not guaranteed to be supported.
    - Lidar
        - Benewake TF Luna
    - Servo
        - Miuzei FS08MD
* Build Instructions
    - cmake -S . -B build
    - cd build
    - make
* Run Instructions
    - Transfer uf2 file from build directory to Raspberry Pi Pico W drive.
* Reading Firmware USB Serial Output
    - Manual
        - Identify serial port
            - sudo dmesg | tail
        - View USB serial output
            - Example: sudo screen /dev/ttyACM0 115200
        - Exit USB serial output
            - Enter keyboard commands: Ctrl+A, k, y
    - Automatic
        - Before loading firmware to the pico:
            - sudo ./record_serial_logs.sh
            - tail -f serial_log.txt


