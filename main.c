#include <stdlib.h>
#include <pigpio.h>
#include <unistd.h>
#include <stdio.h>
#define PWM_PIN 18
#define SLP_TIM 10
const int fr=100000;
int dt=650000;
int getT();
int currT=0;
const int ii=5;
//500 000 - 1 000 000
int main(int argc, char *argv[])
{
   if (gpioInitialise()<0) return 1;
printf("temp_ctl v2\nPWM PIN 18\n");   
gpioHardwarePWM(PWM_PIN, fr, dt);
	   
	while(1) {
		currT=0;
		for (int i=1;i<=ii;i++) {
			currT+=getT();
			sleep(SLP_TIM);		
		}
		currT=currT/ii;
			printf ("T=%dC\n", currT);

		if (currT<44) {
			dt=0;
			gpioHardwarePWM(PWM_PIN, 0, dt);
		}else if (currT>=44 && currT<=46) {
			dt=650000;
			gpioHardwarePWM(PWM_PIN, fr, dt);
		}else if (currT>=47 && currT<=49) {
			dt=770000;
			gpioHardwarePWM(PWM_PIN, fr, dt);			
		}else if (currT>=50 && currT<58) {
			dt=900000;
			gpioHardwarePWM(PWM_PIN, fr, dt);
		}else if (currT>=58) {
			dt=1000000;
			gpioHardwarePWM(PWM_PIN, fr, dt);
		}	
		printf("dt=%d\n",dt); 			
		sleep(SLP_TIM);
	}

   gpioTerminate();
}

int getT() {
	FILE *temperatureFile;
	int T;
	temperatureFile = fopen ("/sys/class/thermal/thermal_zone0/temp", "r");
	if (temperatureFile == NULL) {
		T=48;
		printf ("Error reading /sys/class/thermal/thermal_zone0/temp\n");
	}
	fscanf (temperatureFile, "%d", &T);
	T /= 1000;
	//printf ("T=%dC\n", T);
	fclose (temperatureFile);
	return T;
}
