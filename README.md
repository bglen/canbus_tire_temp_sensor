# canbus_tire_temp_sensor

Tire temperature sensing with IR sensor arrays and a CAN bus interface

![PCB render](/hardware/PCB/production/images/version_1.1.png)

## Usage
This device outputs thermal camera data to CAN bus. It does not include termination resistors, so only include it at the beginning or end of the network with an external termination resistor.

### Pinout

| Pin Number    | Name | Description |
| -------- | ------- |
| 1  | VCC      | Input voltage, 4.5 - 40 VDC|
| 2  | GND      | Ground                     |
| 3  | CAN_N    | CAN bus high               |
| 4  | CAN_P    | CAN bus low                |

### Default CAN Message IDs

| Default ID   | Message | Description |
| -------- | ------- |
| 0x700 + 1,2,3  | Data Messages      | TBD                                         |
| 0x710          | Config Messages    | Adjust Default ID, bitrate, sensor settings |
