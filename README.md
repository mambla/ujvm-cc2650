# ujvm-cc2650

This is a port of [uJ](http://dmitry.gr/index.php?r=05.Projects&proj=12.%20uJ%20-%20a%20micro%20JVM) for the TI CC2650 LAUNCHXL. 

## Features
* Full support of java bytecode including threading, exceptions, and all data types (double, long, float) by the uJ jvm.
* Native and wrapper classed for controlling GPIO, printing to UART, writing to the SPI flash and waiting and reacting to button presses.
* Up to 1Mb of class files can be saved on the SPI flash, which can be reprogrammed over UART
* This port is for the TI CC2650 LAUNCHXL, but can easily be made to work on any TI MCU that is supported by TI-RTOS 

## Directory Structure
* scripts/ - scripts used for concatenating the class files and writing them to the SPI flash over UART
* src/flashprog/ - A small program that can be used to program the SPI flash over UART (without the jvm)
* src/ujvm/ - The uJ jvm port
* src/test/simon/ - contains an implementation of the game of Simon using the CC2650 LAUNCHXL LEDs and buttons
* bin/ - contains the classCvt binary used to optimize class files which was provided as part of [uJ](http://dmitry.gr/index.php?r=05.Projects&proj=12.%20uJ%20-%20a%20micro%20JVM)

## Usage
1. Compile and flash the ujvm project to your board.
2. Write a java project using the framework classes found in src/test/simon.
3. Run **python scripts/process_class_dir.py path_to_java_project_classes_root output_file --class_cvt bin/classCvt** in order to concatenate the class files into one file
3. Run **python flashprog.py output_file** in order to send the java classes to the ujvm (which saves them in the SPI flash).
4. Open some serial terminal (115200, 8, 1, None) in order to see the program prints

## License

Original uJ code and license can be found [here](http://dmitry.gr/index.php?r=05.Projects&proj=12.%20uJ%20-%20a%20micro%20JVM).

