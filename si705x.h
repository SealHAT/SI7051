#ifndef __SI705X_H__
#define __SI705X_H__

#include <atmel_start.h>    /* where the IO functions live */
#include <stdint.h>
#include <stdbool.h>
#include "si705xTypes.h"

/** @brief initializes the temperature sensor data structures
 *
 * This function initializes the data structures and sets the appropriate ports
 * to use on the device. The address of this device is fixed and so is not a
 * parameter.
 *
 * @param WIRE_I2C [IN] The device structure to use for the sensor (I2C Syncronous)
 * @return True if successful, false if not. Any error is likely due to I2C
 */
bool si705x_init(struct i2c_m_async_desc* const WIRE_I2C);

/** @brief Set the resolution of the temperature sensor
 *
 * This function sets the resolution of the sensor to one of four pre-defined settings
 *     - TEMP_RES_11 -> 11 bits of resolution (.085 degrees, max 416Hz)
 *     - TEMP_RES_12 -> 12 bits of resolution (.043 degrees, max 263Hz)
 *     - TEMP_RES_13 -> 13 bits of resolution (.022 degrees, max 161Hz)
 *     - TEMP_RES_14 -> 14 bits of resolution (.011 degrees, max 92Hz)
 *
 * @param RES [IN] The resolution to set the sensor to
 * @return True if successful, false if not. Any error is likely due to I2C
 */
bool si705x_set_resolution(const uint8_t RES);

/** @brief Obtain a temperature measurement from the sensor
 *
 * TODO write description
 *
 * @param MASTER_HOLD [IN] Use master hold mode if true, otherwise do not
 * @return True if successful, false if not. Any error is likely due to I2C
 */
uint16_t si705x_measure(const bool MASTER_HOLD);

/** @brief convert a raw reading into degrees celsius
 *
 * @param READING [IN] raw reading obtained from si507x_measure()
 * @return the temperature in degrees celsius
 */
inline float si705x_celsius(const uint16_t READING) { return ((175.72 * READING) / 65536) - 46.85; }

/** @brief Gets the voltage warning bit from the sensor
 *
 * The sensor will set a bit high if the voltage drops below 1.9 volts, but the
 * sensor will cease to operate at 1.8 volts. Therefore if this function returns
 * false then the voltage is between 1.8V and 1.9V and action should be taken.
 *
 * @return true if the voltage is ok
 */
bool si705x_voltage_ok();

/** @brief Gets the firmware version of the device
 *
 * The device can report it's firmware version. This will be returned as the hex
 * value that the device gives which can be masked with;
 *      - TEMP_FW1 -> Firmware Version 1.0
 *      - TEMP_FW2 -> Firmware Version 2.0
 *
 * @return the hex value of the firmware register.
 */
uint32_t si705x_fw_version();

/** @brief perform a software reset of the device
 *
 * The sensor can be reset using this function. It will be ready to use
 * 15ms (max) from reset, typically 5ms.
 *
 * @return True if successful, false if not. Any error is likely due to I2C
 */
float si705x_reset();

#endif    /* __SI705X_H__ */
