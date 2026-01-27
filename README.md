# NFC12A1 Polling Tag Detect Application

This project demonstrates how to detect various types of NFC/RFID tags using the STMicroelectronics NUCLEO-L476RG development board in conjunction with the X-NUCLEO-NFC12A1 expansion board, which features the ST25R300 high-performance NFC reader.

## Project Overview

The `NFC12A1_PollingTagDetect` application is a demonstration that actively polls for nearby NFC tags. When a tag is brought into the reader's field, the application detects it and can identify its type. This serves as a starting point for more complex NFC applications such as reading/writing NDEF messages, implementing card emulation, or peer-to-peer communication.

The application utilizes the `X-CUBE-NFC12` software expansion for STM32Cube, which provides a comprehensive middleware for the ST25R300 NFC reader, including the RF Abstraction Layer (RFAL).

## Hardware Requirements

To run this demonstration, you will need the following hardware:

*   **NUCLEO-L476RG:** An STM32 Nucleo-64 development board featuring the STM32L476RG microcontroller.
*   **X-NUCLEO-NFC12A1:** An NFC reader expansion board based on the ST25R300. This board connects to the Nucleo board via the Arduino Uno R3 compatible connectors.
*   **NFC Tags:** Various NFC tags to be detected (e.g., Type 1, Type 2, Type 4, Type 5/ISO-15693).
*   **Micro-USB Cable:** To power the Nucleo board and for programming/debugging.

## Hardware Setup

1.  Mount the **X-NUCLEO-NFC12A1** expansion board on top of the **NUCLEO-L476RG** board, ensuring all the pins are correctly aligned with the Arduino connectors.
2.  Connect the NUCLEO-L476RG board to your computer using a Micro-USB cable.

## Pin Configuration

The application uses the SPI interface to communicate between the STM32L476RG MCU and the ST25R300 NFC reader. The following are the key pin connections:

| Pin Name      | NUCLEO-L476RG Pin | Description                  |
|---------------|-------------------|------------------------------|
| **SPI1_SCK**  | `PA5`             | SPI Clock                    |
| **SPI1_MISO** | `PA6`             | SPI Master In Slave Out      |
| **SPI1_MOSI** | `PA7`             | SPI Master Out Slave In      |
| **NFC_CS**    | `PA1`             | SPI Chip Select for ST25R300 |
| **NFC_IRQ**   | `PA0`             | Interrupt from ST25R300      |

Additionally, a UART interface is configured for logging and debugging purposes, typically using `USART2` on the Nucleo board, which is connected to the ST-LINK debugger. This allows you to view log messages on a PC serial terminal.

*   **USART2_TX:** `PA2`
*   **USART2_RX:** `PA3`

## How to Use

1.  **Build the Project:** Compile the project using your preferred toolchain (e.g., STM32CubeIDE, Keil MDK, IAR EWARM).
2.  **Flash the Firmware:** Load the compiled binary onto the NUCLEO-L476RG board.
3.  **Run the Application:** The application will start running automatically after flashing.
4.  **Observe Output:** Open a serial terminal (e.g., Tera Term, PuTTY) with the following settings to view the log messages from the application:
    *   **Baud Rate:** 115200
    *   **Data Bits:** 8
    *   **Parity:** None
    *   **Stop Bits:** 1
5.  **Detect Tags:** Bring an NFC tag close to the antenna of the X-NUCLEO-NFC12A1 board. The terminal will display information about the detected tag.
