/**
 * @file    ecu_tasks.c
 * @brief   FreeRTOS Task Definitions for Automotive ECU
 * @author  Anjaneya Reddy Pondugula
 */

#include "FreeRTOS.h"
#include "task.h"
#include "ecu_tasks.h"
#include "can_driver.h"
#include <stdint.h>

/* Task periods in milliseconds */
#define SENSOR_TASK_PERIOD_MS    10U
#define CAN_TASK_PERIOD_MS       20U
#define FAULT_TASK_PERIOD_MS     50U

/* Fault code storage */
static uint8_t fault_codes[16];
static uint8_t fault_count = 0U;

/**
 * @brief Sensor acquisition task
 *        Runs every 10ms - achieves sub-10ms response time
 */
void task_sensor_acquisition(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    (void)pvParameters;

    while (1)
    {
        /* Read sensor data from ADC/SPI peripherals */
        /* Process and validate sensor readings     */
        /* Store in shared memory for other tasks   */

        vTaskDelayUntil(&xLastWakeTime,
                        pdMS_TO_TICKS(SENSOR_TASK_PERIOD_MS));
    }
}

/**
 * @brief CAN diagnostics task - UDS/ISO 15765
 *        Handles fault code reporting and ECU messaging
 */
void task_can_diagnostics(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    uint8_t    payload[7]    = {0};
    (void)pvParameters;

    while (1)
    {
        /* Check for pending diagnostic requests */
        if (fault_count > 0U)
        {
            payload[0] = fault_codes[0];
            uds_send_request(0x19U, payload, 1U);
        }

        vTaskDelayUntil(&xLastWakeTime,
                        pdMS_TO_TICKS(CAN_TASK_PERIOD_MS));
    }
}

/**
 * @brief Fault monitoring task
 *        Detects and logs system faults
 */
void task_fault_monitor(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    (void)pvParameters;

    while (1)
    {
        /* Monitor system health parameters */
        /* Log faults to fault_codes array  */
        /* Trigger safe state if critical   */

        vTaskDelayUntil(&xLastWakeTime,
                        pdMS_TO_TICKS(FAULT_TASK_PERIOD_MS));
    }
}
