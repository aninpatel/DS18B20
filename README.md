# DS18B20
Performance of Loadable Kernel Module and User Space Module for Temperature Sensor DS18B20 on Raspberry Pi Platform

Refer this file to compile and run the code!

USER LEVEL NON-THREADED: Directory: /user_module/

    Compile: gcc -o user_module user_module.c -lwiringPi -lm
    Run: sudo ./user_module

USER LEVEL THREADED: Directory: /user_thread_module/user_thread_module/

    Compile: gcc -o user_thread_module.c -lwiringPi -lm -pthread
    Run: sudo ./user_thread_module

KERNEL LEVEL NON-THREADED: Directory: /kernel_module/

    Compile: make (Create Makefile)
    Run (Insert the module): sudo insmod kernel_module.ko
    Observe the output: dmesg (last enteries)
    To remove the module: sudo rmmod kernel_module.ko

KERNEL LEVEL THREADED: Directory: /kernel_thread_module/kernel_thread_module/

    Compile: make (Create Makefile)
    Run (Insert the module): sudo insmod kernel_thread_module.ko
    Observe the output: dmesg (last enteries)
    To remove the module: sudo rmmod kernel_thread_module.ko

(Execute all the commands sequentially for each case) (*Note: For observing the kernel level threaded results, wait approximately for 4 to 5 seconds before giving the command dmesg as thread might be running in background after loading of the module) (**Note: To run the kernel module it has to be unloaded (removed) and loaded once again if loaded previously, two kernel module won't work at the same time as the GPIO is used by both)
