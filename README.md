# Hard Landing Detection (HLD) System

An embedded sensor-fusion system developed on the Arduino Uno platform to detect severe structural impacts, hard drops, or unstable flight approaches for drones and unmanned vehicles. The system evaluates real-time data from a suite of environmental and physical sensors to classify landing intensity and display immediate warning notifications.

---

## Hardware Requirements

- **Microcontroller**: Arduino Uno R3
- **Sensors**:
  - HC-SR04 Ultrasonic Distance Sensor (Ground Proximity detection)
  - Flex Sensor (Structural deformation/compression tracking)
  - Force Sensitive Resistor / Force Sensor (Impact magnitude assessment)
  - 2 x Tilt Sensors (Pitch and Roll orientation stabilization flags)
- **Display Module**: I2C 16x2 LCD Screen (MCP23008-based, Address `0x20`)
- **Passive Components**: Resistors ($2 \times 1\text{k}\Omega$, $2 \times 500\Omega$)

---

## Wiring & Hardware Pin Mapping

| Component | Component Pin | Arduino Uno Pin | Description |
| :--- | :--- | :--- | :--- |
| **HC-SR04 Ultrasonic** | VCC | 5V | Power Supply |
| | Trig | Pin 9 | Trigger Pulse Output |
| | Echo | Pin 8 | Echo Pulse Input |
| | GND | GND | Ground |
| **Flex Sensor** | Signal Terminal | Pin A2 | Analog Voltage Divider Input |
| **Force Sensor** | Signal Terminal | Pin A3 | Analog Voltage Divider Input |
| **Tilt Sensor (Pitch)**| Signal Terminal | Pin 2 | Digital Interrupt Input |
| **Tilt Sensor (Roll)** | Signal Terminal | Pin 3 | Digital Interrupt Input |
| **I2C LCD Display** | SDA | Pin A4 | Serial Data Line |
| | SCL | Pin A5 | Serial Clock Line |

---

## Logic Thresholds & Decision Matrix

The embedded firmware evaluates sensor telemetry using the following conditions to prevent redundant alert states:

- `thresholdForce`: 245 (Minimum impact value)
- `thresholdFlex`: 27 (Maximum bending deflection allowance)
- `Distance Threshold`: $\le 100\text{ cm}$ (Ground proximity activation range)

| Distance | Hard Landing Flag State | Flex Sensor Value | Force Sensor Value | System Classification Result |
| :--- | :--- | :--- | :--- | :--- |
| $\le 100\text{ cm}$ | `false` | $\le$ `thresholdFlex` | $\ge$ `thresholdForce` | **Hard Landing Detected** (Trigger Alert) |
| $\le 100\text{ cm}$ | `true` | Any | Any | **No Action** (Alert Already Latched) |
| $> 100\text{ cm}$ | `false` | Any | Any | **No Action** (In-Air Flight Mode) |
