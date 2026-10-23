
#include "config.h"
#include "logger.h"

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CONFIG_BUFFER_SIZE 512U

#define DEFAULT_TEMP_MAX 350
#define DEFAULT_TEMP_MIN 50
#define DEFAULT_HUMI_MAX 800
#define DEFAULT_HUMI_MIN 200

static void config_set_defaults(alert_config_t *p_config)
{
    p_config->temp_max = DEFAULT_TEMP_MAX;
    p_config->temp_min = DEFAULT_TEMP_MIN;
    p_config->humi_max = DEFAULT_HUMI_MAX;
    p_config->humi_min = DEFAULT_HUMI_MIN;
}

static void config_parse_line(char *p_line,
                              alert_config_t *p_config)
{
    char *p_equal = strchr(p_line, '=');

    if (p_equal == NULL)
    {
        LOG_WARN("Malformed config line");
        return;
    }

    *p_equal = '\0';

    const char *p_key = p_line;
    const char *p_value = p_equal + 1;
    char *p_end = NULL;

    errno = 0;
    long value = strtol(p_value, &p_end, 10);

    if ((p_end == p_value) ||
        (errno == ERANGE) ||
        ((*p_end != '\0') && (*p_end != '\r')))
    {
        LOG_WARN("Invalid config value");
        return;
    }

    if (value > INT16_MAX)
    {
        value = INT16_MAX;
    }
    else if (value < INT16_MIN)
    {
        value = INT16_MIN;
    }

    if (strcmp(p_key, "temp_max") == 0)
    {
        p_config->temp_max = (int16_t)value;
    }
    else if (strcmp(p_key, "temp_min") == 0)
    {
        p_config->temp_min = (int16_t)value;
    }
    else if (strcmp(p_key, "humi_max") == 0)
    {
        p_config->humi_max = (int16_t)value;
    }
    else if (strcmp(p_key, "humi_min") == 0)
    {
        p_config->humi_min = (int16_t)value;
    }
    else
    {
        LOG_WARN("Unknown config key: %s", p_key);
    }
}

status_t config_load(const char *p_file_path,
                     alert_config_t *p_config)
{
    if ((p_file_path == NULL) || (p_config == NULL))
    {
        return STATUS_ERR_NULL_PTR;
    }

    if (p_file_path[0] == '\0')
    {
        return STATUS_ERR_INVALID_PARAM;
    }

    config_set_defaults(p_config);

    int fd = open(p_file_path, O_RDONLY);

    if (fd < 0)
    {
        if (errno == ENOENT)
        {
            LOG_WARN("Config missing, using defaults");
            return STATUS_OK;
        }

        return STATUS_ERR_IO;
    }

    char buffer[CONFIG_BUFFER_SIZE];
    size_t used = 0U;
    bool discard_line = false;
    status_t result = STATUS_OK;

    for (;;)
    {
        char ch;
        ssize_t count = read(fd, &ch, 1U);

        if (count == 0)
        {
            break;
        }

        if (count < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }

            result = STATUS_ERR_IO;
            break;
        }

        if (ch == '\n')
        {
            if (!discard_line)
            {
                buffer[used] = '\0';
                if (used > 0U)
                {
                    config_parse_line(buffer, p_config);
                }
            }

            used = 0U;
            discard_line = false;
            continue;
        }

        if (discard_line)
        {
            continue;
        }

        if (used >= sizeof(buffer) - 1U)
        {
            LOG_WARN("Config line too long");
            used = 0U;
            discard_line = true;
            continue;
        }

        buffer[used] = ch;
        used++;
    }

    if ((result == STATUS_OK) &&
        (!discard_line) &&
        (used > 0U))
    {
        buffer[used] = '\0';
        config_parse_line(buffer, p_config);
    }

    if (close(fd) != 0)
    {
        result = STATUS_ERR_IO;
    }

    return result;
}

status_t config_save(const char *p_file_path,
                     const alert_config_t *p_config)
{
    if ((p_file_path == NULL) || (p_config == NULL))
    {
        return STATUS_ERR_NULL_PTR;
    }

    if (p_file_path[0] == '\0')
    {
        return STATUS_ERR_INVALID_PARAM;
    }

    char buffer[CONFIG_BUFFER_SIZE];

    int length = snprintf(buffer, sizeof(buffer),
                          "temp_max=%d\n"
                          "temp_min=%d\n"
                          "humi_max=%d\n"
                          "humi_min=%d\n",
                          (int)p_config->temp_max,
                          (int)p_config->temp_min,
                          (int)p_config->humi_max,
                          (int)p_config->humi_min);

    if ((length < 0) ||
        ((size_t)length >= sizeof(buffer)))
    {
        return STATUS_ERR_IO;
    }

    int fd = open(p_file_path,
                  O_WRONLY | O_CREAT | O_TRUNC,
                  0644);

    if (fd < 0)
    {
        return STATUS_ERR_IO;
    }

    size_t total = 0U;
    size_t length_to_write = (size_t)length;
    status_t result = STATUS_OK;

    while (total < length_to_write)
    {
        ssize_t written = write(fd,
                                buffer + total,
                                length_to_write - total);

        if (written < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }

            result = STATUS_ERR_IO;
            break;
        }

        if (written == 0)
        {
            result = STATUS_ERR_IO;
            break;
        }

        total += (size_t)written;
    }

    if (close(fd) != 0)
    {
        result = STATUS_ERR_IO;
    }

    return result;
}
