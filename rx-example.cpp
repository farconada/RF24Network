#include <cstdlib>
#include <iostream>
#include "RF24.h"
#include "RF24Network.h"

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
struct payload_t
{
  unsigned long ms;
  unsigned long counter;
};

int main(int argc, char** argv) 
{

	// Refer to RF24.h or nRF24L01 DS for settings
	radio.begin();
	delay(1);
	network.begin(/*channel*/ 90, /*node address*/ this_node);
	
	while(1)
	{
		  network.update();
		  while ( network.available() )
		  {
		    // If so, grab it and print it out
		    RF24NetworkHeader header;
		    payload_t payload;
		    network.read(header,&payload,sizeof(payload));
		    printf("Received packet #");
		    printf("%lu", payload.counter);
		    printf(" at ");
		    printf("%lu\n", payload.ms);
		    
		  }
		 //printf("out\n");
		 delay(2000);
	}

	
	return 0;
}

