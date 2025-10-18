

Linux Chararcter Device Driver

Loaded : Bootloader, Kernel and rootfs binaries
Device: Zybo Z7-10 Board

Logs: printk() is used, dmesg for console

PetaLinux is a software development kit (SDK) from AMD (formerly Xilinx) that simplifies building custom embedded Linux systems for AMD's FPGAs and SoCs


Procedure:

Operating systems : Peta Linux which is an embedded Linux Software Development Kit(SDK) is used to build the Linux Kernal and Other required images

Setup: ZYBO Z7-10(Zynq Board), USB Drive and an SD card, Details about Zybo architecture, registers etc.. can be found in manual



Processor : ARM Cortex A9 - Processor present on FPGA Board
Connection: UART is used to connect the USB-UART port(J12) on the FPGA board to computer via a cable

Steps for Development:

1. Remove SD card from Zybo board and connect to PC in the lab. Also connect the USB where Linux project directory and peta Linux project directory created 
2. Create multiplier Kernel module code setup along with the makefile and bitbake script with the commands
3. Write the code by referring examples
4. Compiled the C code mentioned in the manual and build the kernel module
5. After compiling the code, copy the generated kernel modules into the SD card and mount the SD card on Zybo board
6. Load the kernel module and observe the traces
7. Register the device as per the trace resulted from loading the module

Build Test Application:

1. Implement devtest.c and use the code mentioned 
2. Add all missing parts of the code such as opening, closing the device and writing the values to the device file and reading the results
3. Compile the C code as per the steps 
4. After compiling the code, copy the generated binary into the SD card and mount the SD card on Zybo board
5. Run the binary and observe the traces




