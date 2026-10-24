# Changelog

All notable changes to the IoT Environmental Monitoring System project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and the project follows Semantic Versioning.

## [Unreleased]

### Added
- CI/CD and release documentation for the final project.

## [1.0.0] - Pending Release

### Added
- Modular C11 project architecture using CMake and Ninja.
- Shared status types and Design by Contract macros.
- Hardware Abstraction Layer (HAL) with simulated sensor registers and timer.
- Logger with severity levels and file output.
- Temperature and humidity sensor drivers.
- Sensor manager for reading environmental measurements.
- Ring buffer, moving-average processing, and event log.
- TLV telemetry serialization and hexadecimal output.
- Alert management with configurable temperature and humidity thresholds.
- Finite State Machine (FSM) for application control.
- Command-Line Interface (CLI) supporting `start`, `stop`, `status`, `config`, `alert_ack`, `reset`, `help`, and `quit`.
- Configuration loading and saving using POSIX file I/O.
- Default configuration values when the configuration file is missing.
- Unity unit tests for the major project components.
- GitHub Actions workflow for build, static analysis, pre-commit, and sanitizer-enabled testing.
- Architecture documentation with Mermaid diagrams.
- Sprint planning and project retrospective documentation.

### Changed
- Integrated the individual modules into the main application.
- Improved CLI input handling.
- Updated code formatting and addressed cppcheck diagnostics.

### Notes
- Sensor readings are simulated rather than collected from physical hardware.
- Monitoring samples are processed after CLI commands while the system is in the monitoring state.
- The v1.0.0 release remains pending until final quality checks and tagging are completed.