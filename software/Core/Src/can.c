#include "can.h"

static uint32_t s_base_id = CAN_DEFAULT_BASE_ID;
static uint32_t s_baudrate = 1000000U;

uint32_t CANAPP_GetBaseID(void)
{
    return s_base_id;
}

static HAL_StatusTypeDef CANAPP_Setup(uint32_t baudrate)
{
    uint32_t pclk = HAL_RCC_GetPCLK1Freq();
    uint32_t prescaler = pclk / (baudrate * 8U);
    if (prescaler == 0)
    {
        prescaler = 1;
    }

    hcan.Instance = CAN;
    hcan.Init.Prescaler = prescaler;
    hcan.Init.Mode = CAN_MODE_NORMAL;
    hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan.Init.TimeSeg1 = CAN_BS1_6TQ;
    hcan.Init.TimeSeg2 = CAN_BS2_1TQ;
    hcan.Init.TimeTriggeredMode = DISABLE;
    hcan.Init.AutoBusOff = DISABLE;
    hcan.Init.AutoWakeUp = DISABLE;
    hcan.Init.AutoRetransmission = DISABLE;
    hcan.Init.ReceiveFifoLocked = DISABLE;
    hcan.Init.TransmitFifoPriority = DISABLE;

    if (HAL_CAN_Init(&hcan) != HAL_OK)
    {
        return HAL_ERROR;
    }

    CAN_FilterTypeDef filter = {0};
    filter.FilterBank = 0;
    filter.FilterMode = CAN_FILTERMODE_IDMASK;
    filter.FilterScale = CAN_FILTERSCALE_32BIT;
    filter.FilterIdHigh = 0;
    filter.FilterIdLow = 0;
    filter.FilterMaskIdHigh = 0;
    filter.FilterMaskIdLow = 0;
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterActivation = ENABLE;

    if (HAL_CAN_ConfigFilter(&hcan, &filter) != HAL_OK)
    {
        return HAL_ERROR;
    }

    if (HAL_CAN_Start(&hcan) != HAL_OK)
    {
        return HAL_ERROR;
    }

    return HAL_OK;
}

HAL_StatusTypeDef CANAPP_Init(uint32_t base_id, uint32_t baudrate)
{
    s_base_id = base_id;
    s_baudrate = baudrate;
    return CANAPP_Setup(s_baudrate);
}

HAL_StatusTypeDef CANAPP_Send(uint32_t id, uint8_t *data, uint8_t len)
{
    CAN_TxHeaderTypeDef txHeader = {0};
    uint32_t mailbox;
    txHeader.StdId = id;
    txHeader.IDE = CAN_ID_STD;
    txHeader.RTR = CAN_RTR_DATA;
    if (len > 8U)
    {
        return HAL_ERROR;
    }
    txHeader.DLC = len;
    return HAL_CAN_AddTxMessage(&hcan, &txHeader, data, &mailbox);
}

HAL_StatusTypeDef CANAPP_Receive(CAN_RxHeaderTypeDef *header, uint8_t *data)
{
    if (HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0) > 0)
    {
        return HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, header, data);
    }
    return HAL_ERROR;
}

HAL_StatusTypeDef CANAPP_UpdateConfig(uint32_t base_id, uint32_t baudrate)
{
    s_base_id = base_id;
    s_baudrate = baudrate;
    HAL_CAN_Stop(&hcan);
    return CANAPP_Setup(s_baudrate);
}
