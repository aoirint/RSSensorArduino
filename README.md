
## RSSensorArduino
Program for Arduino to send sensor data of a room environment.

### Requirements
- ArduinoJson
  - https://github.com/bblanchon/ArduinoJson

### Parts
- Illuminance sensor
  - (probably) NJL7202L-F3
    - [秋月電子通商](http://akizukidenshi.com/catalog/g/gI-08910/)
- Temperature sensor
  - (probably) LM61CIZ
    - [秋月電子通商](http://akizukidenshi.com/catalog/g/gI-11160/)
- Arduino UNO

### Circuit
```
[5V]-[Long]Illumi Sensor[Short]-T--[PIN A0]
                                L-[200kΩ]--[GND]
```

```
(top view)

            (rounded)
           ___--^--___
[5V]-[Left]Temp Sensor[Right]-[GND]
             [Center]
                 L--[PIN A1]
```

```
[GND]---o [Aluminium pieces(electrode)] o---[PIN 2 (PULLUP)]
```

### Data Format
This program transmits data as json via serial port (baudrate=38400).

#### Sensor data
```json
{
  "type": "sensor",
  "light": 1023,
  "temperature": 1023,
}
```

The values of `light`, `temperature` are examples (Range: [0, 1023]).

#### Door open/closed data
This is only transmitted when door open state is changed.

```json
{
  "type": "doorSensor",
  "doorState": "open",
}
```

`doorState` is `open` or `closed`. This value will be `open` when aluminium pieces connects the circuit.

### Temperature Calculation & Calibration
Temperature can be calculated like below (`temperatureRaw` is `temperature` in json).

```c
#define TMPR_VOLT            5.0
#define TMPR_V_OFFSET        0.6
#define TMPR_DEGREE_PER_VOLT 0.01

float v0 = (float)temperatureRaw / 1023 * TMPR_VOLT;
float temperature = (v0 - TMPR_V_OFFSET) / TMPR_DEGREE_PER_VOLT;
```

Probably some offset (calibration) is required (I needed to subtract about 10℃). Use another thermometer.
