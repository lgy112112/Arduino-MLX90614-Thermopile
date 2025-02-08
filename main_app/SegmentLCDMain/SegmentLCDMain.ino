#include <Arduino.h>
#include "SegmentLCD.h"
#include <Wire.h>
#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup()
{
	unsigned int i;
	Serial.begin(9600);
	Init_1621();
	Serial.println("MLX90614 test");  

    mlx.begin();  

}

void loop()
{
	float obj_temp_1 = mlx.readObjectTempC();
  float obj_temp_2 = mlx.readObjectTempC2();
	Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC()); 
  	Serial.print("*C\tObject = "); Serial.print(obj_temp_1); Serial.println("*C");
    // Serial.print("*C\tObject 2 = "); Serial.print(obj_temp_2); Serial.println("*C");

  	int temp[4] = {0,0,0,0};
  	temp[0] = int(obj_temp_1)/10;			// Tens place
  	temp[1] = int(obj_temp_1)%10;			// Units place
  	temp[2] = int(obj_temp_1*10+0.5)%10;	// First decimal place (rounded)

	Write_1621_data(0,0x00);			
	Write_1621_data(1,0x00);
	Write_1621_data(2,num[temp[0]]|0x10);		// Display tens place with degree symbol
	Write_1621_data(3,num[temp[1]]);			// Display units place	
	Write_1621_data(4,num[temp[2]]|0x10);		// Display first decimal place
	Write_1621_data(5,0xe1);	// Display 'C' for Celsius
	delay(500);

}
