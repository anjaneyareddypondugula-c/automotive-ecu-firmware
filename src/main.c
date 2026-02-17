/**
 * @file    main.c
 * @brief   Automotive ECU Firmware - Main Entry Point
 * @platform ARM Cortex-M / STM32
 * @author  Anjaneya Reddy Pondugula
 */

#include "FreeRTOS.h"
#include "task.h"
#include "ecu_tasks.h"
#include "can_driver.h"
#include "uart_driver.h"

#define SYS_CLOCK_HZ    168000000UL

static void hw_init(void)
{
    uart_init(115200);
    can_init(500000);
}

int main(void)
{
    hw_init();

    xTaskCreate(task_sensor_acquisition, "SensorAcq", 256, NULL, 3, NULL);
    xTaskCreate(task_can_diagnostics,    "CANDiag",   256, NULL, 2, NULL);
    xTaskCreate(task_fault_monitor,      "FaultMon",  256, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1) {}
    return 0;
}
