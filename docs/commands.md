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
