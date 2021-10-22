# Wiring

| Signal | Arduino pin |
| --- | --- |
| Left motor direction | 2 and 3 |
| Right motor direction | 4 and 5 |
| Left motor speed (PWM) | 10 |
| Right motor speed (PWM) | 11 |
| Bluetooth module | 10 and 11 through SoftwareSerial |

Pins 10 and 11 appear twice because the sketch declares the Bluetooth link and the PWM outputs on the same pins. Commands currently arrive on the hardware serial port (`Serial`), so connect the Bluetooth module to the board's RX and TX pins or move one of the two uses to a free pin.
