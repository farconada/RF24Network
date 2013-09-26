#include <cstdlib>
#include <iostream>
#include "RF24.h"
#include "RF24Network.h"
#include <ctime>
#include <stdio.h>
#include <rrd.h>
/**
 * g++ -L/usr/lib main.cc -I/usr/include -o main -lrrd
 **/
using namespace std;

// CE Pin, CSN Pin, SPI Speed

// Setup for GPIO 22 CE and GPIO 25 CSN with SPI Speed @ 1Mhz
//RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_18, BCM2835_SPI_SPEED_1MHZ);

// Setup for GPIO 22 CE and CE0 CSN with SPI Speed @ 4Mhz
//RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_4MHZ); 

// Setup for GPIO 22 CE and CE1 CSN with SPI Speed @ 8Mhz
RF24 radio(RPI_V2_GPIO_P1_12, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);  
RF24Network network(radio);

// Address of our node
const uint16_t this_node = 0;

// Address of the other node
const uint16_t other_node = 1;

// Structure of our payload
struct payload_power_t
{
  unsigned long  nodeId;
  float power;
  float current;
};

struct payload_weather_t
{
  unsigned long  nodeId;
  float temperature;
  float humidity;
  unsigned long  lux;
};
int main(int argc, char** argv) 
{
	// Refer to RF24.h or nRF24L01 DS for settings
	char    *rrd_argv[2];
    	int       rrd_argc;
    	char filenameTemp[] = "/root/weather/weather_sensor.rrd";
    	char filenameCurrent[] = "/root/weather/current_sensor.rrd";
    	char command[] = "update";
	char values[100];

	radio.begin();
	radio.setDataRate(RF24_250KBPS);
	radio.setRetries(7,7);
	delay(5);
	network.begin(/*channel*/ 50, /*node address*/ this_node);
	while(1)
	{
		  //FILE * pFile;
	          //pFile = fopen ("/root/temp-exterior.txt","a");
		  network.update();
		  while ( network.available() )
		  {
		    // If so, grab it and print it out
		    RF24NetworkHeader header;
		    network.peek(header);

		    // sensor de temperatura y humedad
		    if (header.from_node == 1) {
		    	    payload_weather_t payload;
		    	    network.read(header,&payload,sizeof(payload));
			    rrd_argv[0]= command; 
			    rrd_argv[1]= filenameTemp; 
			    rrd_argv[3]= NULL;
			    rrd_argc=3;
			    printf("N:%.2f:%.2f:%lu\n", payload.temperature, payload.humidity, payload.lux);    
			    sprintf(values, "N:%.2f:%.2f:%lu", payload.temperature, payload.humidity, payload.lux);    
			    rrd_argv[2]= values;
			    rrd_update(rrd_argc, rrd_argv);
		    }

		    if (header.from_node == 2) {
		    	    payload_power_t payload;
		    	    network.read(header,&payload,sizeof(payload));
			    rrd_argv[0]= command; 
			    rrd_argv[1]= filenameCurrent; 
			    rrd_argv[3]= NULL;
			    rrd_argc=3;
			    printf("N:%.2f:%.2f\n", payload.power, payload.current);    
			    sprintf(values, "N:%.2f:%.2f", payload.power, payload.current);    
			    rrd_argv[2]= values;
			    rrd_update(rrd_argc, rrd_argv);
			    
		    }
		    //time_t timer;
    		    //char timeBuffer[25];
    		    //struct tm* tm_info;
    		    //time(&timer);
    		    //tm_info = localtime(&timer);
		    //strftime(timeBuffer, 25, "%Y/%m/%d %H:%M:%S", tm_info);
		    //fprintf(pFile, "%s;%lu;%.2f;%.2f\n", timeBuffer,payload.nodeId, payload.data1, payload.data2);    
		  }
		 delay(2000);
		 //fclose(pFile);
	}

	return 0;
}

