# Infrared Thermometer Project with MLX90614 and Segment LCD

## Project Overview

This project implements a non-contact infrared thermometer using an Arduino UNO, an MLX90614 IR temperature sensor, and a custom segment LCD driven by an HT1621.  The thermometer displays both object and ambient temperatures in real-time with a resolution of 0.1°C.

## Key Features

1.  **Dual Temperature Measurement:**
    *   Object temperature range: -70°C to +380°C
    *   Ambient temperature range: -40°C to +85°C
    *   Accuracy: ±0.5°C at room temperature

2.  **Measurement Algorithm:**
    Raw sensor data is converted to Celsius using the following formula:
    ```
    Temperature = (raw_value * 0.02) - 273.15
    ```

3.  **Display System:**
    *   6-digit segment LCD with custom glyph encoding.
    *   Supports special characters (e.g., °C symbol).
    *   Refresh rate: 500ms.

4.  **Sensor Configuration:**
    *   Fixed I2C address: 0x5A
    *   Field of view: 90°
    *   Factory calibrated outputs.

## Technical Specifications

| Parameter           | Value                       |
| ------------------- | ---------------------------- |
| Power Supply        | 4.5-5.5V DC                 |
| Measurement Range   | -70°C to +380°C (Object)     |
| Accuracy            | ±0.5°C @ room temperature   |
| Field of View       | 90°                         |
| Resolution          | 0.02°C                      |
| Refresh Rate        | 500ms                       |

## Hardware Interface

### LCD Connections:

*   DATA: Pin 9
*   WR: Pin 8
*   CS: Pin 7

### I2C Configuration:

*   SDA/SCL: Arduino default pins (A4/A5 on UNO)
*   7-bit address: 0x5A

## Dependencies

*   Arduino IDE
*   Wire Library (for I2C communication)
*   Adafruit MLX90614 Library (modified -  Note: The original document mentions a *modified* version.  If there are specific modifications, consider including a link to your modified library or a description of the changes here.)

## Usage

1.  Connect the hardware components according to the pin definitions provided above.
2.  Upload the firmware to the Arduino UNO.
3.  Power cycle the device.
4.  Point the sensor at the target object.
5.  Read the temperature from the LCD display.

## Documentation Reference

*   **MLX90614 Datasheet:**  Consult the datasheet for complete sensor specifications, electrical characteristics, and communication protocol details.