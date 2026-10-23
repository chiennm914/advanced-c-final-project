
# Sprint Plan — IoT Environmental Monitoring System

## 1. Project Overview

**Project:** IoT Environmental Monitoring System  
**Version:** 1.0.0  
**Language:** C11  
**Build System:** CMake + Ninja  
**Testing:** Unity  
**Quality Tools:** clang-format, clang-tidy, cppcheck, ASan/UBSan  
**Repository:** https://github.com/chiennm914/advanced-c-final-project

### Project Objectives

Develop a modular IoT environmental monitoring application that:

- Simulates temperature and humidity sensors.
- Processes sensor readings using data structures.
- Detects environmental threshold violations.
- Encodes sensor data into telemetry packets.
- Provides CLI commands and FSM-based control.
- Loads and saves configuration data.
- Uses automated testing and static analysis.
- Includes architecture and release documentation.

## 2. Work Breakdown Structure (WBS)

| WBS | Work Package | Deliverables |
|---|---|---|
| 1.0 | Project Foundation | Git repository, CMake, project structure |
| 1.1 | Common Infrastructure | Common types and Design by Contract |
| 1.2 | HAL | Simulated registers and timer |
| 2.0 | Logging | Logger module |
| 3.0 | Sensor Drivers | Temperature, humidity, sensor manager |
| 4.0 | Data Processing | Ring buffer, moving average, event log |
| 5.0 | Telemetry | TLV serialization |
| 6.0 | Application Control | FSM, CLI, alert management |
| 7.0 | System Integration | Configuration persistence, integrated main |
| 8.0 | Delivery | CI/CD, documentation, release v1.0.0 |

## 3. Product Backlog

| ID | User Story / Task | Priority | Acceptance Criteria |
|---|---|---|---|
| IOT-001 | Initialize modular C project | High | CMake build, HAL simulation and initial tests |
| IOT-002 | Implement logger | High | Logging levels, console/file output, unit tests |
| IOT-003 | Implement sensor drivers | High | Sensor APIs and simulated readings |
| IOT-004 | Implement data structures | High | Ring buffer, moving average, event log |
| IOT-005 | Implement telemetry | High | TLV serialization and tests |
| IOT-006 | Implement FSM, CLI and alerts | High | State transitions, CLI commands, threshold checks |
| IOT-007 | Integrate system and configuration | High | Config load/save, integrated monitoring and tests |
| IOT-008 | Add CI/CD and documentation | High | Four quality gates, architecture, sprint plan, changelog, README and release |

## 4. Two-Week Sprint Schedule

The following is the planned two-week schedule, rather than a verified record of actual completion dates.

### Sprint 1 — Foundation and Core Components

**Duration:** Week 1 (Days 1–7)

**Sprint Goal:** Establish the development environment and implement reusable system components.

| Day | Planned Work | Task |
|---|---|---|
| 1 | Repository setup, CMake, project structure | IOT-001 |
| 2 | HAL simulator, timer and unit tests | IOT-001 |
| 3 | Logger implementation and tests | IOT-002 |
| 4 | Sensor drivers and sensor manager | IOT-003 |
| 5 | Ring buffer and moving-average processing | IOT-004 |
| 6 | Event log and telemetry encoding | IOT-004, IOT-005 |
| 7 | Integration review, unit testing and fixes | IOT-001–005 |

**Planned Deliverables:**

- Modular C11 project.
- Working HAL and sensor simulation.
- Logger, data processing and telemetry modules.
- Initial Unity test coverage.

### Sprint 2 — Application Integration and Release

**Duration:** Week 2 (Days 8–14)

**Sprint Goal:** Integrate system behavior, validate quality and prepare version 1.0.0.

| Day | Planned Work | Task |
|---|---|---|
| 8 | FSM and state transition logic | IOT-006 |
| 9 | CLI commands and alert handling | IOT-006 |
| 10 | Configuration load/save implementation | IOT-007 |
| 11 | Main application integration | IOT-007 |
| 12 | Integration testing and static analysis | IOT-007 |
| 13 | CI workflow and architecture documentation | IOT-008 |
| 14 | Sprint plan, changelog, README and release preparation | IOT-008 |

**Planned Deliverables:**

- Integrated CLI-based monitoring application.
- Alert and configuration modules.
- Unit and integration checks.
- GitHub Actions CI.
- Architecture and project documentation.
- Release candidate v1.0.0.

## 5. Definition of Done (DoD)

A task is considered complete when:

- [ ] Implementation matches the task requirements.
- [ ] Code follows C11 and project coding conventions.
- [ ] CMake builds without compilation errors.
- [ ] Relevant Unity unit tests pass.
- [ ] clang-tidy checks pass.
- [ ] cppcheck reports no blocking findings.
- [ ] Pre-commit formatting checks pass.
- [ ] Sanitizer-enabled tests pass where applicable.
- [ ] Documentation is updated.
- [ ] Changes are committed and reviewed through the Git workflow.

For IOT-008, all four CI quality gates must pass before declaring the release ready.

## 6. Git Branching Strategy

The project uses a feature-based Git workflow:

```text
main
 |
 +-- develop
      |
      +-- feature/IOT-001-project-scaffolding
      +-- feature/IOT-002-logger
      +-- feature/IOT-003-sensor-drivers
      +-- feature/IOT-004-data-processing
      +-- feature/IOT-005-telemetry
      +-- feature/IOT-006-app-logic
      +-- feature/IOT-007-system-integration
      +-- feature/IOT-008-ci-docs-release
```

Feature branches are integrated into `develop`. A `release/v1.0.0` branch is used for final validation before merging into `main` and creating the `v1.0.0` tag.

## 7. Risk Management

| Risk | Impact | Mitigation |
|---|---|---|
| Compiler differences between Windows and Linux | CI failures | Validate using GCC and GitHub Actions |
| Static-analysis version differences | Unexpected warnings | Review diagnostics and maintain compatible code |
| Memory errors | Crashes or undefined behavior | Use Unity tests and ASan/UBSan |
| Incorrect FSM transitions | Invalid application behavior | Test transition cases |
| Configuration file errors | Invalid thresholds | Apply defaults and validate parsing |
| Integration delays | Release delay | Prioritize core functionality and quality gates |

## 8. Sprint Review

### Implemented Functionality

The current application demonstrates:

- HAL-based simulated sensor readings.
- CLI commands for starting, stopping and inspecting the system.
- FSM transitions between system states.
- Alert configuration display.
- TLV telemetry output.
- Logger initialization and shutdown.
- Configuration defaults when the file is missing.

### Validation Evidence

Local cppcheck completed analysis of 22 project translation units without blocking diagnostics after the Task 8 fixes.

The GitHub Actions workflow has demonstrated successful Build + clang-tidy and Unity Tests + ASan/UBSan runs. Final CI validation of all four gates remains pending.

## 9. Sprint Retrospective

### What Went Well

- Modular components made the application easier to integrate.
- Unit testing helped verify individual features.
- Git feature branches separated development tasks.
- Static analysis identified code quality issues before release.
- The CLI provided a practical way to demonstrate system behavior.

### Challenges

- Differences between Windows/MSYS2 and Ubuntu tooling.
- clang-format version differences between local development and CI.
- cppcheck warnings related to variable scope and callback signatures.
- Integration and Git synchronization required additional verification.

### Improvements for Future Iterations

- Standardize formatter and static-analysis tool versions.
- Run quality checks before pushing each feature branch.
- Add more integration tests for alert and configuration scenarios.
- Consider timer-driven sampling independent of CLI input.
- Improve automated release validation.

## 10. Release Readiness

Version 1.0.0 is ready for release only after:

- [ ] All required documentation is committed.
- [ ] All four CI quality gates pass.
- [ ] The release branch is validated.
- [ ] Changes are merged into `main`.
- [ ] The `v1.0.0` Git tag is created and pushed.
- [ ] The public repository link is included in README.md.
