#include <wiringPi.h>

int main()
{
	wiringPiSetup();
	for(int i = 0; i < 28; i++){
		pinMode(i, OFF);
	}
//	pinMode(1, OFF);
	return 0;
}
