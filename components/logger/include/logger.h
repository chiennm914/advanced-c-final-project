#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>

/**
 * @brief Logging severity levels.
 */
typedef enum {
    LOG_LEVEL_DEBUG = 0U,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} log_level_t;

#define LOG_DEBUG(...) \
    do { \
        logger_log(LOG_LEVEL_DEBUG, __FILE__, __LINE__, __func__, \
                   __VA_ARGS__); \
    } while (0)

#define LOG_INFO(...) \
    do { \
        logger_log(LOG_LEVEL_INFO, __FILE__, __LINE__, __func__, \
                   __VA_ARGS__); \
    } while (0)

#define LOG_WARN(...) \
    do { \
        logger_log(LOG_LEVEL_WARN, __FILE__, __LINE__, __func__, \
                   __VA_ARGS__); \
    } while (0)

#define LOG_ERROR(...) \
    do { \
        logger_log(LOG_LEVEL_ERROR, __FILE__, __LINE__, __func__, \
                   __VA_ARGS__); \
    } while (0)


    /**
 * @brief Initialize the logger.
 *
 * @param[in] min_level Minimum log level to output.
 * @param[in] p_file_path Path to the log file.
 */
void logger_init(log_level_t min_level, const char *p_file_path);

/**
 * @brief Write a formatted log message.
 *
 * @param[in] level Log severity level.
 * @param[in] p_file Source file name.
 * @param[in] line Source line number.
 * @param[in] p_func Source function name.
 * @param[in] p_fmt printf-style format string.
 */
void logger_log(log_level_t level,
                const char *p_file,
                uint32_t line,
                const char *p_func,
                const char *p_fmt,
                ...);

/**
 * @brief Close the logger.
 */
void logger_close(void);

#endif /* LOGGER_H */