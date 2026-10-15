
#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <stdint.h>
#include "common_types.h"
#include "sensor_mgr.h"

#define TLV_SYNC_BYTE       (0xAAU)
#define TLV_MAX_PACKET_SIZE (8U)
#define TLV_HEADER_SIZE     (4U)

/**
 * @brief Serialize temperature and humidity into TLV packets.
 *
 * @param[in] p_data Pointer to sensor readings.
 * @param[out] p_buf Output byte buffer.
 * @param[in] buf_size Capacity of output buffer.
 * @param[out] p_written Number of bytes written.
 * @return STATUS_OK on success, error status otherwise.
 */
status_t telemetry_serialize(const sensor_data_t *p_data,
                             uint8_t *p_buf,
                             uint32_t buf_size,
                             uint32_t *p_written);

/**
 * @brief Deserialize TLV packets into sensor readings.
 *
 * @param[in] p_buf Input byte buffer.
 * @param[in] buf_len Length of input buffer.
 * @param[out] p_data Recovered sensor readings.
 * @return STATUS_OK on success, error status otherwise.
 */
status_t telemetry_deserialize(const uint8_t *p_buf,
                               uint32_t buf_len,
                               sensor_data_t *p_data);

/**
 * @brief Calculate XOR checksum.
 *
 * @param[in] p_data Input bytes.
 * @param[in] len Number of bytes.
 * @return XOR checksum.
 */
uint8_t telemetry_crc8(const uint8_t *p_data, uint32_t len);

/**
 * @brief Print telemetry buffer in hexadecimal format.
 *
 * @param[in] p_buf Input buffer.
 * @param[in] len Number of bytes.
 */
void telemetry_print_hex(const uint8_t *p_buf, uint32_t len);

#endif /* TELEMETRY_H */
