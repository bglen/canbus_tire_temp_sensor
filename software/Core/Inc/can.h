#ifndef CAN_H
#define CAN_H

#include "stm32f0xx_hal.h"

#define CAN_CONFIG_MSG_ID   0x710U
#define CAN_DEFAULT_BASE_ID 0x700U

extern CAN_HandleTypeDef hcan;

/** Initialize CAN peripheral and application parameters */
HAL_StatusTypeDef CANAPP_Init(uint32_t base_id, uint32_t baudrate);

/** Send a CAN data frame */
HAL_StatusTypeDef CANAPP_Send(uint32_t id, uint8_t *data, uint8_t len);

/** Receive a CAN data frame if available */
HAL_StatusTypeDef CANAPP_Receive(CAN_RxHeaderTypeDef *header, uint8_t *data);

/** Update CAN base ID and baud rate */
HAL_StatusTypeDef CANAPP_UpdateConfig(uint32_t base_id, uint32_t baudrate);

/** Get current base ID used for transmissions */
uint32_t CANAPP_GetBaseID(void);

#endif /* CAN_H */
