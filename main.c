#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include <inc/hw_memmap.h>

#include <driverlib/crc.h>
#include <driverlib/rom.h>
#include <driverlib/rom_map.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/systick.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>
#include <driverlib/watchdog.h>

#include "buttons.h"
#include "rgb.h"

#define GET_DEVICE_REQUEST 0x74
#define GET_DEVICE_RESPONSE 0x75

#define DEVICE_ID_SIZE 6

static uint8_t g_deviceEnterDFU[] = { 0x00, 0x00, 0x02, 0x00, 0x00, 0x00 };
static uint8_t g_deviceReset[]    = { 0xE0, 0x0A, 0x00, 0x00, 0x00, 0x00 };
static uint8_t g_defaultDCSD[]    = { 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 };

static uint8_t* g_currentDevice = NULL;

void SendWake(void) {

}


void DoResetCommand(void) {
    g_currentDevice = g_deviceReset;
    SendWake();
}

void DoDFUCommand(void) {
    g_currentDevice = g_deviceEnterDFU;
    SendWake();
}

//*****************************************************************************
//
// Handler to manage the button press events and state machine transitions
// that result from those button events.
//
//*****************************************************************************
void AppButtonHandler(void)
{
    //
    // Switch statement to adjust the color wheel position based on buttons
    //

}

//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void ConfigureUART(void)
{
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    MAP_GPIOPinConfigure(GPIO_PA0_U0RX);
    MAP_GPIOPinConfigure(GPIO_PA1_U0TX);
    MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}


uint8_t CalculateCRC(uint8_t* data, uint8_t length)
{
    uint8_t bit = 0;
    uint8_t crc = 0;
    uint8_t byteCtr;

    for (byteCtr = 0; byteCtr < length; ++byteCtr)
    {
        crc ^= (data[byteCtr]);

        for (bit = 8; bit > 0; --bit)
        {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x31;
            }
            else {
                crc = (crc << 1);
            }
        }
    }

    return 0;
}

void ConfigureWatchdog() {
    // Step one - did the dog bite us?



    //
    // Enable the peripherals used by this example.
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);

}


/**
 * main.c
 */
int main(void)
{
    //
    // Enable and configure the Watchdog to reset the state if we get hung during a bit-bang
    //
    ConfigureWatchdog();

    //
    // Enable and Initialize the UART.
    //
    ConfigureUART();

    //
    // Set the system clock to run at 16Mhz off PLL with external crystal as
    // reference.
    //
    MAP_SysCtlClockSet(SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

	return 0;
}
