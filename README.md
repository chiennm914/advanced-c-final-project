# Advanced C final project

# IoT Environmental Monitoring System

<!-- FINAL_PROJECT_REPO: https://github.com/chiennm914/advanced-c-final-project -->

**Advanced C — Final Project**  
**Version:** 1.0.0  
**Language:** C11  
**Build System:** CMake + Ninja  
**Testing Framework:** Unity

**GitHub Repository:** https://github.com/chiennm914/advanced-c-final-project

## 1. Project Overview

IoT Environmental Monitoring System is a modular C11 application that simulates temperature and humidity measurements, processes sensor data, checks alert thresholds, and generates telemetry packets.

The application uses a Command-Line Interface (CLI) and Finite State Machine (FSM) to manage monitoring operations.

This project demonstrates Advanced C programming concepts, modular software architecture, automated testing, and static code analysis.

## 2. Features

- Simulated temperature and humidity sensors.
- Hardware Abstraction Layer (HAL) with simulated registers.
- Timer tick simulation.
- Logger with different severity levels.
- Sensor manager and sensor drivers.
- Ring buffer and moving-average data processing.
- Event logging.
- TLV telemetry serialization.
- Configurable temperature and humidity alert thresholds.
- Finite State Machine (FSM).
- Command-Line Interface (CLI).
- Configuration loading and saving.
- Unit testing with Unity.
- Static analysis using clang-tidy and cppcheck.
- GitHub Actions continuous integration.
- AddressSanitizer and UndefinedBehaviorSanitizer testing.

## 3. Project Structure

```text
advanced-c-final-project/
├── .github/
│   └── workflows/
│       └── ci.yml
├── app/
│   ├── CMakeLists.txt
│   └── main.c
├── components/
│   ├── common/
│   ├── hal/
│   ├── logger/
│   ├── drivers/
│   ├── data/
│   ├── telemetry/
│   ├── alert/
│   ├── app/
│   └── config/
├── test/
│   └── CMakeLists.txt
├── docs/
│   ├── ARCHITECTURE.md
│   └── SPRINT_PLAN.md
├── .pre-commit-config.yaml
├── CMakeLists.txt
├── CHANGELOG.md
└── README.md
```

## 4. Requirements

### Development Tools

- GCC with C11 support
- CMake 3.14 or later
- Ninja
- Git

### Quality and Testing Tools

- clang-tidy
- clang-format
- cppcheck
- Python and pre-commit (for local pre-commit checks)

The Unity testing framework is downloaded automatically by CMake using FetchContent, so an existing internet connection is needed during initial configuration.

## 5. Build Instructions

### Windows (MSYS2 UCRT64 + Git Bash)

Open the project in VS Code and use a terminal with GCC, CMake, and Ninja available in PATH.

Configure:

```bash
cmake -S . -B build -G Ninja
```

Build:

```bash
cmake --build build
```

Run:

```bash
./build/app/iot_monitor.exe
```

### Linux (Ubuntu)

Install the required build tools:

```bash
sudo apt update
sudo apt install -y build-essential cmake ninja-build git clang-tidy cppcheck
```

Configure and build:

```bash
cmake -S . -B build -G Ninja
cmake --build build
```

Run:

```bash
./build/app/iot_monitor
```

## 6. CLI Commands

After launching the application, enter commands at the `IoT>` prompt.

| Command | Description |
|---|---|
| `help` | Display available commands |
| `start` | Start monitoring |
| `stop` | Stop monitoring |
| `status` | Display current system state |
| `config` | Display alert threshold configuration |
| `alert_ack` | Acknowledge an active alert |
| `reset` | Request a system reset |
| `quit` | Exit the application |

### Example

```text
=== IoT Environmental Monitoring System v1.0.0 ===
Type 'help' to see available commands.

IoT> help
Available commands: start stop status config alert_ack reset help quit

IoT> status
State: IDLE

IoT> start
State: MONITORING
Temp=22.4C Humi=80.0% tick=1

IoT> stop
IoT> quit
Shutting down... Goodbye!
```

The example is illustrative. Actual readings are simulated and may vary between runs.

## 7. System States

The application uses the following states:

| State | Description |
|---|---|
| `SYS_INIT` | System initialization |
| `SYS_IDLE` | System ready but not monitoring |
| `SYS_MONITORING` | Sensor monitoring enabled |
| `SYS_ALERT` | Alert condition detected |
| `SYS_ERROR` | Error state |

State transitions are managed by the FSM component.

## 8. Configuration

The application attempts to load configuration from `config.txt`.

If the file is unavailable, default thresholds are used.

Default values are stored in tenths of a unit:

| Parameter | Raw Value | Actual Value |
|---|---:|---:|
| Temperature minimum | 50 | 5.0 °C |
| Temperature maximum | 350 | 35.0 °C |
| Humidity minimum | 200 | 20.0% |
| Humidity maximum | 800 | 80.0% |

The `config` command displays the currently loaded threshold values.

## 9. Testing

The project uses the Unity unit testing framework.

Build the project first:

```bash
cmake -S . -B build -G Ninja
cmake --build build
```

Run all tests:

```bash
ctest --test-dir build --output-on-failure
```

Test targets include:

- `test_hal`
- `test_logger`
- `test_sensors`
- `test_data`
- `test_telemetry`
- `test_app`
- `test_config`

## 10. Static Analysis

### clang-tidy

clang-tidy is integrated into the CMake build when the executable is available in PATH.

```bash
cmake --build build
```

### cppcheck

Run the custom CMake quality gate:

```bash
cmake --build build --target cppcheck
```

### Pre-commit

Install pre-commit in a compatible Python environment, then run:

```bash
pre-commit run --all-files
```

The repository includes hooks for clang-format and clang-tidy.

## 11. Sanitizer Build

On a supported GCC/Clang environment, configure a separate sanitizer build:

```bash
cmake -S . -B build-sanitizer -G Ninja -DSANITIZER=asan+ubsan
cmake --build build-sanitizer
ctest --test-dir build-sanitizer --output-on-failure
```

Sanitizer support depends on the selected compiler and platform.

## 12. Continuous Integration

GitHub Actions workflow:

`.github/workflows/ci.yml`

The CI pipeline includes four quality gates:

1. Build + clang-tidy
2. Pre-commit checks
3. cppcheck
4. Unity tests with ASan/UBSan

All required gates must pass before release.

## 13. Architecture

The project uses a modular architecture.

Major components include:

- **HAL:** Simulated hardware and timer.
- **Drivers:** Sensor acquisition.
- **Data:** Sample storage and processing.
- **Telemetry:** TLV packet serialization.
- **Alert:** Threshold checking.
- **App:** FSM and CLI.
- **Config:** Configuration persistence.
- **Logger:** Diagnostic logging.

For more information, see [Architecture Documentation](docs/ARCHITECTURE.md).

## 14. Development Process

The project is organized into eight development tasks, from project initialization to release preparation.

See [Sprint Plan](docs/SPRINT_PLAN.md) for:

- Work Breakdown Structure (WBS)
- Product Backlog
- Two-week Sprint Plan
- Definition of Done
- Sprint Review
- Sprint Retrospective

Version history is available in [CHANGELOG.md](CHANGELOG.md).

## 15. Release

**Target release:** `v1.0.0`

Release workflow:

```text
feature branches
       |
       v
    develop
       |
       v
release/v1.0.0
       |
       v
      main
       |
       v
   tag v1.0.0
```

The release is finalized after documentation review and successful CI checks.

## 16. Limitations

- Sensor readings are simulated rather than collected from physical hardware.
- Monitoring is driven by CLI interaction rather than an independent periodic sampling thread.
- This project is intended as an educational Advanced C application.

## 17. Repository

Source code:

https://github.com/chiennm914/advanced-c-final-project
