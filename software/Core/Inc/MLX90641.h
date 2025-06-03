#ifndef MLX90641_H
#define MLX90641_H

/**
 * @file MLX90641.h
 * @brief Minimal driver interface for the MLX90641 infrared array.
 *
 * This header provides the function prototypes and basic defines to
 * communicate with the MLX90641 sensor using the STM32 HAL I2C API.
 */

#include "stm32f0xx_hal.h"

/**
 * @brief MLX90641 7-bit slave address shifted for HAL (8-bit)
 */
#define MLX90641_I2C_ADDR       (0x33 << 1)

/**
 * @brief Number of temperature pixels in the sensor output (16 x 12)
 */
#define MLX90641_NUM_PIXELS     192

/**
 * @brief Initialize the sensor and start measurements.
 *
 * The sensor is put in measurement mode with a 4 Hz refresh rate
 * as described in the device datasheet.
 *
 * @param hi2c Pointer to initialized I2C handle.
 * @retval HAL status code
 */
HAL_StatusTypeDef MLX90641_Init(I2C_HandleTypeDef *hi2c);

/**
 * @brief Read all temperature pixels from the device.
 *
 * @param hi2c  I2C handle used for communication.
 * @param buffer Destination array for pixel temperatures in degrees Celsius.
 *               Must have at least @ref MLX90641_NUM_PIXELS elements.
 * @retval HAL status code
 */
HAL_StatusTypeDef MLX90641_ReadTemperatures(I2C_HandleTypeDef *hi2c, float *buffer);

#endif /* MLX90641_H */
