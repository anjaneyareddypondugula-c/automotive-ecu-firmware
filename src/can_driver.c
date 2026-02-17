/**
 * @file    can_driver.c
 * @brief   CAN Bus Driver - UDS/ISO 15765 Support
 * @author  Anjaneya Reddy Pondugula
 */

#include "can_driver.h"
#include <stdint.h>
#include <string.h>

#define CAN_MAX_DLC        8U
#define CAN_TIMEOUT_MS     100U
#define UDS_FUNC_ADDR      0x7DFU
#define UDS_PHYS_ADDR      0x7E0U

typedef struct {
    uint32_t id;
    uint8_t  dlc;
    uint8_t  data[CAN_MAX_DLC];
} CAN_Frame_t;

static CAN_Frame_t tx_frame;
static CAN_Frame_t rx_frame;

void can_init(uint32_t baudrate)
{
    /* Configure CAN peripheral registers */
    /* Set baudrate prescaler based on system clock */
    (void)baudrate;
}

CAN_Status_t can_send(uint32_t id, uint8_t *data, uint8_t len)
{
    if (data == NULL || len > CAN_MAX_DLC) {
        return CAN_ERROR;
    }

    tx_frame.id  = id;
    tx_frame.dlc = len;
    memcpy(tx_frame.data, data, len);

    /* Trigger hardware transmission */
    return CAN_OK;
}

CAN_Status_t can_receive(uint32_t *id, uint8_t *data, uint8_t *len)
{
    if (id == NULL || data == NULL || len == NULL) {
        return CAN_ERROR;
    }

    *id  = rx_frame.id;
    *len = rx_frame.dlc;
    memcpy(data, rx_frame.data, rx_frame.dlc);

    return CAN_OK;
}

/* UDS - Send diagnostic request (ISO 15765) */
CAN_Status_t uds_send_request(uint8_t service_id, uint8_t *payload, uint8_t len)
{
    uint8_t frame[CAN_MAX_DLC] = {0};
    frame[0] = len + 1U;   /* PCI byte */
    frame[1] = service_id;

    if (payload && len > 0U) {
        memcpy(&frame[2], payload, len);
    }

    return can_send(UDS_PHYS_ADDR, frame, CAN_MAX_DLC);
}
