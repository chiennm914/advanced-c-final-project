
# IoT Environmental Monitoring System — Architecture

## 1. System Overview

The IoT Environmental Monitoring System is a modular application written in C11. It simulates temperature and humidity sensors, processes sensor data, checks alert thresholds, serializes telemetry packets, and provides a command-line interface.

The project uses CMake, Unity, clang-tidy, cppcheck, and GitHub Actions.

## 2. Use Case Diagram

```mermaid
flowchart LR
    User["User / Operator"]

    subgraph System["IoT Monitoring System"]
        UC1(["Start monitoring"])
        UC2(["Stop monitoring"])
        UC3(["View system status"])
        UC4(["View alert configuration"])
        UC5(["Acknowledge alert"])
        UC6(["Reset system"])
        UC7(["Quit application"])
    end

    User --> UC1
    User --> UC2
    User --> UC3
    User --> UC4
    User --> UC5
    User --> UC6
    User --> UC7
```

## 3. Component Diagram

```mermaid
flowchart TB
    CLI["CLI Commands"]
    FSM["Application FSM"]
    MAIN["Main Application"]
    CONFIG["Configuration"]
    LOGGER["Logger"]
    HAL["HAL Simulator / Timer"]
    SENSOR["Sensor Manager / Drivers"]
    DATA["Data Processing / Ring Buffer"]
    ALERT["Alert Manager"]
    TELEMETRY["Telemetry TLV"]

    MAIN --> CLI
    CLI --> FSM
    MAIN --> CONFIG
    MAIN --> LOGGER
    MAIN --> HAL
    MAIN --> SENSOR
    MAIN --> DATA
    MAIN --> ALERT
    MAIN --> TELEMETRY
    SENSOR --> HAL
    ALERT --> SENSOR
    TELEMETRY --> SENSOR
```

## 4. Sequence Diagram

The following diagram describes one monitoring sample after the user enters a CLI command while the system is monitoring.

```mermaid
sequenceDiagram
    actor User
    participant CLI
    participant Main
    participant HAL
    participant Sensor
    participant Data
    participant Alert
    participant Telemetry
    participant FSM

    User->>CLI: Enter start
    CLI->>FSM: EVT_CMD_START
    FSM-->>CLI: SYS_MONITORING
    CLI-->>Main: Updated application state
    Main->>HAL: hal_sim_update()
    Main->>HAL: hal_timer_tick()
    Main->>Sensor: sensor_mgr_read_all()
    Sensor->>HAL: Read simulated registers
    HAL-->>Sensor: Raw values
    Sensor-->>Main: Temperature and humidity
    Main->>Data: Add samples to filters
    Main->>Alert: alert_check()
    alt Alert triggered
        Main->>FSM: EVT_ALERT_TRIGGERED
        FSM-->>Main: SYS_ALERT
    end
    Main->>Telemetry: telemetry_serialize()
    Telemetry-->>Main: TLV packet
    Main->>Telemetry: telemetry_print_hex()
```

## 5. State Machine Diagram

```mermaid
stateDiagram-v2
    [*] --> SYS_INIT
    SYS_INIT --> SYS_IDLE: EVT_INIT_OK
    SYS_INIT --> SYS_ERROR: EVT_INIT_FAIL
    SYS_IDLE --> SYS_MONITORING: EVT_CMD_START
    SYS_MONITORING --> SYS_IDLE: EVT_CMD_STOP
    SYS_MONITORING --> SYS_ALERT: EVT_ALERT_TRIGGERED
    SYS_MONITORING --> SYS_ERROR: EVT_SENSOR_FAIL
    SYS_ALERT --> SYS_IDLE: EVT_ALERT_ACK
    SYS_ERROR --> SYS_INIT: EVT_CMD_RESET
```

The state diagram presents the principal transitions. The full FSM transition table is implemented in `components/app/src/app_fsm.c`.

## 6. Module Responsibilities

| Module | Responsibility |
|---|---|
| Common | Shared types, status codes, design-by-contract macros |
| HAL | Simulated hardware registers and timer ticks |
| Logger | Diagnostic messages and file logging |
| Drivers | Temperature/humidity drivers and sensor manager |
| Data | Ring buffer, moving-average processing, event log |
| Telemetry | TLV serialization and hexadecimal output |
| Alert | Threshold checking and alert acknowledgement |
| App | Finite-state machine and CLI command dispatch |
| Config | Load/save alert thresholds through POSIX file I/O |
| Main | Initialization, integration, monitoring cycle and shutdown |

## 7. Advanced C Concepts

| Concept | Project application |
|---|---|
| Modular programming | Separate components with public headers |
| Pointers | Sensor output parameters, context pointers |
| Function pointers | CLI command dispatch table |
| Structures and enums | Sensor data, configuration, FSM states |
| Static storage | HAL simulation state |
| Bitwise operations | Hardware registers and telemetry encoding |
| Data structures | Ring buffer and event log |
| Design by Contract | REQUIRE, ENSURE, INVARIANT macros |
| File I/O | Logger and configuration persistence |
| State machines | System control and alert handling |
| Unit testing | Unity test executables |
| Static analysis | clang-tidy and cppcheck |
| Memory safety | AddressSanitizer and UndefinedBehaviorSanitizer |

## 8. Build and Test

```bash
cmake -S . -B build -G Ninja
cmake --build build
ctest --test-dir build --output-on-failure
cmake --build build --target cppcheck
```

The project is tested using Unity and checked through automated GitHub Actions workflows.

## 9. Implementation Notes

The application currently uses a CLI-driven monitoring loop. Sensor sampling occurs after a CLI command when the application state is `SYS_MONITORING`; it is not an independent periodic background sampling loop.

Configuration defaults are used when the configuration file is missing.
