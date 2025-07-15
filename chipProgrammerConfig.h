#ifndef ChipProgrammerConfig_h
#define ChipProgrammerConfig_h


#define BOARD_PICO           0
#define BOARD_ADAFRUIT_ITSY  1
#define BOARD_SPOKE_RP2040   2
#define BOARD_QMTECH_RP2040_DAUGHTERBOARD 3
#define BOARD_WERKZEUG       4
#define BOARD_RP2040_ZERO    5

// Select the board type from the above
#define BOARD_TYPE BOARD_PICO
//#define BOARD_TYPE BOARD_ADAFRUIT_ITSY
//#define BOARD_TYPE BOARD_SPOKE_RP2040
//#define BOARD_TYPE BOARD_WERKZEUG
//#define BOARD_TYPE BOARD_QMTECH_RP2040_DAUGHTERBOARD
//#define BOARD_TYPE BOARD_RP2040_ZERO

// General mapping
// TDI  SPIO RX
// TDO  SPIO TX
// TCK  SPIO SCK
// TMS  SPIO CS
// RST  GPIO
// TRST GPIO

// Set CDC_UART_INTF_COUNT to 0 to disable USB-CDC-UART bridge

#if ( BOARD_TYPE == BOARD_PICO )

#define PIN_TDI 16 
#define PIN_TDO 17
#define PIN_TCK 18
#define PIN_TMS 19
#define PIN_RST 20
#define PIN_TRST 21

#define LED_INVERTED   0
#define PIN_LED_TX     23
#define PIN_LED_ERROR  25
#define PIN_LED_RX     24

#define CDC_UART_INTF_COUNT 1
#define PIN_UART0 uart0
#define PIN_UART0_TX    12
#define PIN_UART0_RX    13


#endif // BOARD_TYPE

#endif // DirtyJtagConfig_h
