* Description
    - This is a project that builds a program for the Raspberry Pi Pico W for mapping the interior of rooms.
* Supported Physical Hardware
    - Note that this project is designed to use specific hardware devices. Alternative or equivalent hardware devices are not guaranteed to be supported.
    - Lidar
        - Benewake TF Luna
        - https://files.seeedstudio.com/wiki/Grove-TF_Mini_LiDAR/res/SJ-PM-TF-Luna-A03-Product-Manual.pdf
        - https://www.amazon.ca/dp/B088BBJ9SQ?ref=ppx_yo2ov_dt_b_fed_asin_title
    - Servo
        - Miuzei FS08MD
        - https://www.amazon.ca/dp/B0CCCY92VP?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1
* Build Instructions
    - cmake -S . -B build
    - cd build
    - make
* Run Instructions
    - Transfer uf2 file from build directory to Raspberry Pi Pico W drive.
* Reading Test Firmware USB Serial Output
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


