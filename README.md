# Bluetooth-Controlled-Car-with-Arduino

![image](https://user-images.githubusercontent.com/35656849/134706157-12f5392e-f5ee-40fe-ac8b-bc8e236005be.png)

An Arduino car that is driven by text commands sent from a phone over a Bluetooth module.

## Commands

| Command | Action |
| --- | --- |
| `forward` | Drive both motors forward |
| `backward` | Drive both motors in reverse |
| `left` | Turn left by slowing the left side |
| `right` | Turn right by slowing the right side |
| `stop` | Stop both motors |
| a number, for example `200` | Set the base speed (0 to 255) |

## Pins

- Left motor direction: 2 and 3, speed (PWM): 10
- Right motor direction: 4 and 5, speed (PWM): 11

Open `bluetoothcontrolledcar.ino` in the Arduino IDE, upload it, then pair the
Bluetooth module with your phone and send the commands above.
