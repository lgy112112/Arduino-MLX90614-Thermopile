#include <Arduino.h>
#include "SegmentLCD.h"
#include <Wire.h>
#include <Adafruit_MLX90614.h>
// 添加 MAX6675 相关库
#include <Thermocouple.h>
#include <MAX6675_Thermocouple.h>

// 定义 MAX6675 引脚
#define SCK_PIN 10
#define CS_PIN 9
#define SO_PIN 8

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
// 创建 MAX6675 对象
Thermocouple* thermocouple;

byte calculatePECRead(byte i2c_address, byte addr, uint16_t data) {
    byte crc = 0;
    byte dataBytes[5];

    dataBytes[0] = i2c_address & 0xFE; // I2C address with write bit (0)
    dataBytes[1] = addr;
    dataBytes[2] = i2c_address | 0x01;  // I2C address with read bit (1)
    dataBytes[3] = data & 0xFF;
    dataBytes[4] = data >> 8;

    for (int i = 0; i < 5; i++) {
        crc ^= dataBytes[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x07;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

uint16_t readEEPROM(uint8_t addr) {
    uint16_t ret;
    Serial.print("Reading from address: 0x");
    Serial.println(addr, HEX);

    Wire.beginTransmission(MLX90614_I2CADDR);
    Wire.write(addr);
    byte error = Wire.endTransmission(false); // 注意这里是 false
     Serial.print("Wire.endTransmission(false) returned: ");
    Serial.println(error);

    Wire.requestFrom(MLX90614_I2CADDR, (uint8_t)3);
    ret = Wire.read();
    ret |= Wire.read() << 8;
    byte pec_received =  Wire.read(); // PEC
    
    // Calculate PEC for read data
    byte pec_calculated = calculatePECRead(MLX90614_I2CADDR << 1 | 0x01, addr, ret);
    Serial.print("Read Value (HEX): 0x");
    Serial.println(ret,HEX);
     Serial.print("PEC received (HEX): 0x");
    Serial.println(pec_received,HEX);
     Serial.print("PEC calculated (HEX): 0x");
    Serial.println(pec_calculated,HEX);
    return ret;
}

// PEC 计算函数 (写入时使用)
byte calculatePEC(byte addr, uint16_t data) {
  byte crc = 0;
  byte dataBytes[4];

  // MLX90614 I2C address (shifted left by 1) + write bit (0)
  dataBytes[0] = MLX90614_I2CADDR << 1;
  dataBytes[1] = addr;          // EEPROM address
  dataBytes[2] = data & 0xFF;   // Data low byte
  dataBytes[3] = data >> 8;     // Data high byte

  for (int i = 0; i < 4; i++) {
    crc ^= dataBytes[i];
    for (int j = 0; j < 8; j++) {
      if (crc & 0x80) {
        crc = (crc << 1) ^ 0x07; // Polynomial: x^8 + x^2 + x + 1
      } else {
        crc <<= 1;
      }
    }
  }
  return crc;
}

void writeEEPROM(uint8_t addr, uint16_t data) {
  Serial.print("Writing to address: 0x");
  Serial.println(addr, HEX);
  Serial.print("Data (HEX): 0x");
  Serial.println(data, HEX);
  // 1. 擦除 EEPROM 地址
  Wire.beginTransmission(MLX90614_I2CADDR);
  Wire.write(addr);
  Wire.write(0x00); // Low byte
  Wire.write(0x00); // High byte
    byte erase_pec = calculatePEC(addr, 0x0000); // 计算擦除操作的 PEC
  Wire.write(erase_pec);
  Wire.endTransmission(true);
  delay(10); // 擦除操作后延时，datasheet推荐至少10ms

  // 2. 计算 PEC
  byte pec = calculatePEC(addr, data);
  Serial.print("Calculated PEC: 0x");
  Serial.println(pec, HEX);

  // 3. 写入数据和 PEC
  Wire.beginTransmission(MLX90614_I2CADDR);
  Wire.write(addr);
  Wire.write(data & 0xFF);
  Wire.write(data >> 8);
  Wire.write(pec);
  byte error = Wire.endTransmission(true);

  Serial.print("Wire.endTransmission() returned: ");
  Serial.println(error);

  delay(10); // 写入操作后延时,datasheet推荐至少10ms
}

void setEmissivity(double emissivity) {
  // 1. 范围检查 (根据您的需要调整范围)
  if (emissivity < 0.0) {
    emissivity = 0.0;
    Serial.println("Warning: Emissivity clamped to 0.0");
  } else if (emissivity > 1.0) {
    emissivity = 1.0;
    Serial.println("Warning: Emissivity clamped to 1.0");
  }

  // 2. 转换为整数
  uint16_t emissivity_int = round(65535.0 * emissivity);

  // 3. (形式上的) 转换为十六进制 - 方便理解，实际上不需要
  //  (省略)

  // 4. 写入 EEPROM
  Serial.print("Setting emissivity to: ");
  Serial.println(emissivity);
  Serial.print("  (0x");
  Serial.print(emissivity_int, HEX);
  Serial.println(")");
  writeEEPROM(MLX90614_EMISS, emissivity_int);
}

void setup()
{
	unsigned int i;
	Serial.begin(9600);
	Wire.begin();
	Init_1621();
	Serial.println("\n\nMLX90614 and MAX6675 test");  

    mlx.begin();
    // 初始化 MAX6675 传感器
    thermocouple = new MAX6675_Thermocouple(SCK_PIN, CS_PIN, SO_PIN);

    // 设置发射率为 0.92
    setEmissivity(0.83);

    // 读取并验证发射率
    uint16_t emissivity = mlx.readEEPROM(MLX90614_EMISS);
    Serial.print("Emissivity after setting: 0x");
    Serial.println(emissivity, HEX);
	
	delay(1000);

     // 验证是否写入成功
    // emissivity = mlx.readEEPROM(MLX90614_EMISS);
    // Serial.print("Emissivity after write: 0x");
    // Serial.println(emissivity, HEX);

}

void loop()
{
	// 读取 MLX90614 温度
	unsigned long currentMillis = millis();
	float obj_temp_1 = mlx.readObjectTempC();
  	// float obj_temp_2 = mlx.readObjectTempC2();
    float ambient_temp = mlx.readAmbientTempC();
    float max6675_temp = thermocouple->readCelsius();
	Serial.print("[");
	Serial.print(currentMillis);
	Serial.print(" ms] MLX90614 - Ambient = "); 
	Serial.print(ambient_temp); 
  	Serial.print("*C\tObject = "); 
	Serial.print(obj_temp_1); 
	Serial.print("*C\tMAX6675 = ");
	Serial.print(max6675_temp);
	Serial.println("*C");
    // Serial.print("*C\tObject 2 = "); Serial.print(obj_temp_2); Serial.println("*C");

	// 读取并打印 MAX6675 温度
	// float max6675_temp = thermocouple->readCelsius();
	// Serial.print("MAX6675 - Temperature = "); 
	// Serial.print(max6675_temp);
	// Serial.println("*C");

  // uint16_t emissivity = mlx.readEEPROM(MLX90614_EMISS);
  // emissivity = mlx.readEEPROM(MLX90614_EMISS);
  //     Serial.print("Emissivity after write: 0x");
  //     Serial.println(emissivity, HEX);
	  
  // 	int temp[4] = {0,0,0,0};
  // 	temp[0] = int(obj_temp_1)/10;			// Tens place
  // 	temp[1] = int(obj_temp_1)%10;			// Units place
  // 	temp[2] = int(obj_temp_1*10+0.5)%10;	// First decimal place (rounded)

	// Write_1621_data(0,0x00);			
	// Write_1621_data(1,0x00);
	// Write_1621_data(2,num[temp[0]]|0x10);		// Display tens place with degree symbol
	// Write_1621_data(3,num[temp[1]]);			// Display units place	
	// Write_1621_data(4,num[temp[2]]|0x10);		// Display first decimal place
	// Write_1621_data(5,0xe1);	// Display 'C' for Celsius
	delay(250);
}