# Order follows dependency rule: each layer depends only on layers above it.
add_subdirectory(common)
add_subdirectory(hal)

# Add more as you implement them in later tasks:
add_subdirectory(logger)
# add_subdirectory(drivers)
# add_subdirectory(data)
# add_subdirectory(telemetry)
# add_subdirectory(alert)
# add_subdirectory(config)