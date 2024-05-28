#gcc hummer1.c -lwiringPi -lwiringPiDev -lpthread -lm -lcrypt -lrt
echo $0
echo $1
gcc $1 -lwiringPi -lwiringPiDev -lpthread -lm -lcrypt -lrt
