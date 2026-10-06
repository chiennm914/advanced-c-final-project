#include "logger.h"

#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>


static int s_log_fd = -1;
static log_level_t s_min_level = LOG_LEVEL_DEBUG;

static const char *logger_level_to_string(log_level_t level)
{
    const char *p_level_str;

    switch (level) {
        case LOG_LEVEL_DEBUG:
            p_level_str = "DEBUG";
            break;

        case LOG_LEVEL_INFO:
            p_level_str = "INFO";
            break;

        case LOG_LEVEL_WARN:
            p_level_str = "WARN";
            break;

        case LOG_LEVEL_ERROR:
            p_level_str = "ERROR";
            break;

        default:
            p_level_str = "UNKNOWN";
            break;
    }

    return p_level_str;
}

void logger_init(log_level_t min_level, const char *p_file_path)
{
    if (p_file_path == NULL) {
        return;
    }

    s_min_level = min_level;

    s_log_fd = open(p_file_path,
                    O_WRONLY | O_CREAT | O_TRUNC,
                    0644);
    if (s_log_fd < 0) {
        (void)fputs("Logger file open failed\n", stderr);
    }
}

void logger_log(log_level_t level,
                const char *p_file,
                uint32_t line,
                const char *p_func,
                const char *p_fmt,
                ...)
{
    int message_length;
    int log_length;
    char message[256];
    char log_buffer[512];
    const char *p_level_str;
    va_list args;

    if (level < s_min_level) {
        return;
    }

    if ((p_file == NULL) ||
        (p_func == NULL) ||
        (p_fmt == NULL)) {
        return;
    }

    va_start(args, p_fmt);
    message_length = vsnprintf(message, sizeof(message), p_fmt, args);
    va_end(args);

    if (message_length < 0) {
        return;
    }
    p_level_str = logger_level_to_string(level);

    log_length = snprintf(log_buffer,
                sizeof(log_buffer),
                "[%s] %s:%u (%s): %s\n",
                p_level_str,
                p_file,
                (unsigned int)line,
                p_func,
                message);
    if (log_length < 0) {
        return;
    }

    if ((size_t)log_length >= sizeof(log_buffer)) {
        log_length = (int)(sizeof(log_buffer) - 1U);
    }
    (void)printf("%s", log_buffer);
    if (s_log_fd >= 0) {
        ssize_t bytes_written;

        bytes_written = write(s_log_fd,
                            log_buffer,
                            (size_t)log_length);

        if (bytes_written < 0) {
            (void)fputs("Logger file write failed\n", stderr);
        }
    }
}

void logger_close(void)
{
    if (s_log_fd >= 0) {
        if (close(s_log_fd) != 0) {
            (void)fputs("Logger file close failed\n", stderr);
        }

        s_log_fd = -1;
    }
}