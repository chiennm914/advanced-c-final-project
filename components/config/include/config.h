
#ifndef CONFIG_H
#define CONFIG_H

#include "common_types.h"
#include "alert.h"

/**
 * @brief Load alert thresholds from a configuration file.
 *
 * Reads key=value pairs from the specified file.
 * If the file does not exist, default values are used.
 *
 * @param p_file_path Path to the configuration file.
 * @param p_config Pointer to the output configuration.
 * @return status_t Operation status.
 */
status_t config_load(const char *p_file_path,
                     alert_config_t *p_config);

/**
 * @brief Save alert thresholds to a configuration file.
 *
 * Writes the configuration using key=value format.
 *
 * @param p_file_path Path to the configuration file.
 * @param p_config Pointer to the configuration to save.
 * @return status_t Operation status.
 */
status_t config_save(const char *p_file_path,
                     const alert_config_t *p_config);

#endif /* CONFIG_H */
