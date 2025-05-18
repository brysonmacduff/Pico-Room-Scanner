* Description
    - This is a project that builds a program for the Raspberry Pi Pico W for mapping the interior of rooms.
* Supported Physical Hardware
    - Note that this project is designed to use specific hardware devices. Alternative or equivalent hardware devices are not guaranteed to be supported.
    - Lidar
        - Benewake TF Luna
    - Servo
        - Miuzei FS08MD
* Dependencies
    - CMake
    - GCC
    - Pico-Core
        - https://github.com/brysonmacduff/Pico-Core
    - GTest
    - spdlog
* Build Configuration
    - Modify the following cmake variables in the top-level CMakeLists.txt file according to your development environment.
        - PICO_SDK_PATH
        - PICO_CORE_LIB_PATH
        - PICO_CORE_INC_PATH
* Build Instructions
    - This project is separated into two distinct cmake projects. 
        - The "app" project contains c++ source code that is safe to use across target platforms. 
            - The "app" project supports unit testing via GoogleTest.
        - The "hal" (hardward abstraction layer) project interfaces with the Raspberry Pi Pico SDK.
            - The hal builds the project firmware (using app project source code) that can be deployed to the Pico W.
    - CLI Commands
        - cd hal
        - cmake -S . -B build
        - cd build
        - make
* Run Instructions
    - Transfer uf2 file from the hal src build directory to Raspberry Pi Pico W drive.
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
* Analysis Instructions
    - This project includes scripts for analyzing scan results. See the README under the "analysis" directory.
* Author Contact Information
    - brysonmacduff@gmail.com
* Youtube Demo
    - https://youtube.com/shorts/Rv179-mPsFM?feature=share
