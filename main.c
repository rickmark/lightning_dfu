#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <crc.h>
#include <rom.h>
#include <rom_map.h>
#include <sysctl.h>
#include <systick.h>

#define GET_DEVICE_REQUEST 0x74
#define GET_DEVICE_RESPONSE 0x75

#define DEVICE_ID_SIZE 6

static uint8_t g_deviceEnterDFU[] = { 0x00, 0x00, 0x02, 0x00, 0x00, 0x00 };
static uint8_t g_deviceReset[]    = { 0xE0, 0x0A, 0x00, 0x00, 0x00, 0x00 };
static uint8_t g_defaultDCSD[]    = { 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 };

static uint8_t* g_currentDevice = NULL;

void SendWake() {

}


void DoResetCommand() {
    g_currentDevice = g_deviceReset;
    SendWake();
}

void DoDFUCommand() {
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
    switch(g_sAppState.ui32Buttons & ALL_BUTTONS)
    {
    case LEFT_BUTTON:
        DoResetCommand();
        break;
    case RIGHT_BUTTON:
        DoDFUCommand();
        break;
    }
}

//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void
ConfigureUART(void)
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


uint8_t CalculateCRC(uint8_t* data, uint8_t length) {
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
