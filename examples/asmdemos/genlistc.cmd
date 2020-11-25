arm-none-eabi-gcc -c -g %1.c 
arm-none-eabi-objdump --source %1.o > %1.lst
