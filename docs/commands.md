# Commands

The sketch reads a whole command from the serial port and compares it as text.

| Text | Result |
| --- | --- |
| `forward` | Both motors forward |
| `backward` | Both motors reverse |
| `left` | Turn left (the text may contain other words) |
| `right` | Turn right (the text may contain other words) |
| `stop` | Brake |
| a number | Set the base speed, 0 to 255 |

Turns change the two wheel speeds by `turn` (50) in opposite directions. Values are clamped to the PWM range 0 to 255.

Every recognised command is answered with `ok <name>`, for example `ok forward`, so the app can show that the car heard it.

## Failsafe

While the car is moving it expects a new command at least every two seconds (`COMMAND_TIMEOUT_MS`). When the phone goes out of range or the app stops sending, the car brakes by itself. Send `forward` again to keep it going. A speed value or `stop` does not count as moving, so they never start the timer.
