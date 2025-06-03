#include "MLX90641.h"

/**
 * @file MLX90641.c
 * @brief Minimal driver implementation for the MLX90641 infrared array.
 *
 * The routines here provide just enough functionality to start the
 * sensor in continuous measurement mode and read back the raw
 * temperature values.  The math for converting to degrees Celsius is
 * based on the simplified formula from the datasheet and does not
 * include full calibration handling.
 */

#define MLX90641_STATUS_REG        0x8000
#define MLX90641_CONTROL_REG1      0x800D
#define MLX90641_RAM_START         0x0400
#define MLX90641_RAM_LENGTH        (MLX90641_NUM_PIXELS + 3) /* pixels + Ta/Vdd */

/**
 * @brief Write a 16-bit value to a register.
 *
 * Small helper used internally to send the register address and data in
 * one I2C transaction.
 */
static HAL_StatusTypeDef MLX90641_Write16(I2C_HandleTypeDef *hi2c, uint16_t reg, uint16_t value)
{
    uint8_t buf[4];
    buf[0] = reg >> 8;
    buf[1] = reg & 0xFF;
    buf[2] = value >> 8;
    buf[3] = value & 0xFF;
    return HAL_I2C_Master_Transmit(hi2c, MLX90641_I2C_ADDR, buf, 4, HAL_MAX_DELAY);
}

/**
 * @brief Read a block of 16-bit registers from the device.
 *
 * @param hi2c Initialized I2C handle
 * @param reg  Starting register address
 * @param len  Number of 16-bit words to read
 * @param dest Buffer to store the read data
 */
static HAL_StatusTypeDef MLX90641_ReadBlock(I2C_HandleTypeDef *hi2c, uint16_t reg, uint16_t len, uint16_t *dest)
{
    uint8_t addr[2];
    addr[0] = reg >> 8;
    addr[1] = reg & 0xFF;
    if (HAL_I2C_Master_Transmit(hi2c, MLX90641_I2C_ADDR, addr, 2, HAL_MAX_DELAY) != HAL_OK)
        return HAL_ERROR;
    return HAL_I2C_Master_Receive(hi2c, MLX90641_I2C_ADDR, (uint8_t*)dest, len * 2, HAL_MAX_DELAY);
}

/**
 * @brief Configure the MLX90641 for continuous measurement.
 *
 * This routine sets a fixed refresh rate of 4 Hz and enables the
 * measurement mode bits as described in the datasheet.
 */
HAL_StatusTypeDef MLX90641_Init(I2C_HandleTypeDef *hi2c)
{
    HAL_StatusTypeDef ret;

    /* Set refresh rate to 4 Hz */
    ret = MLX90641_Write16(hi2c, MLX90641_CONTROL_REG1, 0x1900);
    if (ret != HAL_OK)
        return ret;

    /* Start measurements by setting the start bit in the status register */
    return MLX90641_Write16(hi2c, MLX90641_STATUS_REG, 0x0030);
}

/**
 * @brief Read temperature values from all pixels.
 *
 * The conversion from raw values to degrees Celsius here is very
 * simplistic and only intended for demonstration.  A production driver
 * should apply the full calibration data supplied by the sensor.
 */
HAL_StatusTypeDef MLX90641_ReadTemperatures(I2C_HandleTypeDef *hi2c, float *buffer)
{
    uint16_t raw[MLX90641_RAM_LENGTH];

    if (MLX90641_ReadBlock(hi2c, MLX90641_RAM_START, MLX90641_NUM_PIXELS, raw) != HAL_OK)
        return HAL_ERROR;

    for (uint16_t i = 0; i < MLX90641_NUM_PIXELS; ++i)
    {
        int16_t val = (int16_t)raw[i];
        buffer[i] = (float)val * 0.02f - 273.15f;
    }

    return HAL_OK;
}

