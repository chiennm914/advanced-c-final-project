#include "telemetry.h"

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>


/**
 * @brief Pack a signed 16-bit value into big-endian bytes.
 */
static void pack_int16_be(uint8_t *p_buf, int16_t value)
{
    uint16_t raw = (uint16_t)value;

    p_buf[0] = (uint8_t)(raw >> 8U);
    p_buf[1] = (uint8_t)(raw & 0xFFU);
}

static int16_t unpack_int16_be(const uint8_t *p_buf)
{
    uint16_t raw = ((uint16_t)p_buf[0] << 8U)
                 | (uint16_t)p_buf[1];

    if (raw <= INT16_MAX)
    {
        return (int16_t)raw;
    }

    return (int16_t)((int32_t)raw - 65536);
}

/**
 * @brief Calculate XOR checksum over a byte buffer.
 *
 * @param[in] p_data Pointer to input bytes.
 * @param[in] len Number of bytes.
 * @return Calculated XOR checksum.
 */
uint8_t telemetry_crc8(const uint8_t *p_data, uint32_t len)
{
    uint8_t crc = 0U;

    if (p_data == NULL)
    {
        return 0U;
    }

    for (uint32_t i = 0U; i < len; i++)
    {
        crc ^= p_data[i];
    }

    return crc;
}

status_t telemetry_serialize(const sensor_data_t *p_data,
                             uint8_t *p_buf,
                             uint32_t buf_size,
                             uint32_t *p_written)
{
    if ((p_data == NULL) || (p_buf == NULL) || (p_written == NULL))
    {
        return STATUS_ERR_NULL_PTR;
    }

    *p_written = 0U;

    if (buf_size < 14U)
    {
        return STATUS_ERR_INVALID_PARAM;
    }
    /* Serialize temperature packet */
    p_buf[0] = TLV_SYNC_BYTE;
    p_buf[1] = 5U;
    p_buf[2] = (uint8_t)SENSOR_TEMPERATURE;
    p_buf[3] = 2U;

    pack_int16_be(&p_buf[4], p_data->temperature);

    p_buf[6] = telemetry_crc8(&p_buf[2], 4U);
    /* Serialize humidity packet */
    p_buf[7]  = TLV_SYNC_BYTE;
    p_buf[8]  = 5U;
    p_buf[9]  = (uint8_t)SENSOR_HUMIDITY;
    p_buf[10] = 2U;

    pack_int16_be(&p_buf[11], p_data->humidity);

    p_buf[13] = telemetry_crc8(&p_buf[9], 4U);

    *p_written = 14U;

    return STATUS_OK;
}


status_t telemetry_deserialize(const uint8_t *p_buf,
                               uint32_t buf_len,
                               sensor_data_t *p_data)
{
    if ((p_buf == NULL) || (p_data == NULL))
    {
        return STATUS_ERR_NULL_PTR;
    }

    if (buf_len != 14U)
    {
        return STATUS_ERR_INVALID_PARAM;
    }

    /* Check SYNC bytes */
    if ((p_buf[0] != TLV_SYNC_BYTE) ||
        (p_buf[7] != TLV_SYNC_BYTE))
    {
        return STATUS_ERR_INVALID_PARAM;
    }

    
    /* Check packet lengths and data lengths */
    if ((p_buf[1] != 5U) ||
        (p_buf[8] != 5U) ||
        (p_buf[3] != 2U) ||
        (p_buf[10] != 2U))
    {
        return STATUS_ERR_INVALID_PARAM;
    }

    /* Check sensor types */
    if ((p_buf[2] != (uint8_t)SENSOR_TEMPERATURE) ||
        (p_buf[9] != (uint8_t)SENSOR_HUMIDITY))
    {
        return STATUS_ERR_INVALID_PARAM;
    }
    
    /* Verify checksums */
    if ((p_buf[6] != telemetry_crc8(&p_buf[2], 4U)) ||
        (p_buf[13] != telemetry_crc8(&p_buf[9], 4U)))
    {
        return STATUS_ERR_INVALID_PARAM;
    }
    
    /* Recover sensor values */
    p_data->temperature = unpack_int16_be(&p_buf[4]);
    p_data->humidity = unpack_int16_be(&p_buf[11]);

    return STATUS_OK;
}


void telemetry_print_hex(const uint8_t *p_buf, uint32_t len)
{
    if (p_buf == NULL)
    {
        return;
    }

    for (uint32_t i = 0U; i < len; i++)
    {
        char hex_str[4];

        (void)snprintf(hex_str, sizeof(hex_str),
                       "%02X ", (unsigned int)p_buf[i]);

        (void)printf("%s", hex_str);
    }

    (void)printf("\n");
}


