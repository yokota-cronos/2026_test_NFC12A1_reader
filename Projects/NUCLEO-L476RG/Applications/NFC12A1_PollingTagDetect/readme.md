::: {.row}
::: {.col-sm-12 .col-lg-8}
**NFC12A1 Polling Tag Detect**
------------------------------

This application detects different type of NFC tag

Example Description:

This directory contains a set of source files that implement a simple example based on st25r300. {#this-directory-contains-a-set-of-source-files-that-implement-a-simple-example-based-on-st25r500.}
------------------------------------------------------------------------------------------------

WARNING: When starting the project from Example Selector in STM32CubeMX
and regenerating it from ioc file, you may face a build issue. To solve
it, assuming you started the project for the Nucleo-L476RG board, remove
from the IDE project the file stm32l4xx\_nucleo.c in the
Application/User virtual folder and delete, from Src and Inc folders,
the files: stm32l4xx\_nucleo.c, stm32l4xx\_nucleo.h,
stm32l4xx\_nucleo\_bus.c, stm32l4xx\_nucleo\_bus.h and
stm32l4xx\_nucleo\_errno.h. The same operations apply if you starts the
project for another STM32 Nucleo board (e.g. for Nucleo-L053R8, the
files to be removed are stm32l0xx\_nucleo.c, stm32l0xx\_nucleo.c,
stm32l0xx\_nucleo.h, stm32l0xx\_nucleo\_bus.c, stm32l0xx\_nucleo\_bus.h
and stm32l0xx\_nucleo\_errno.h). ------------------------------------

### **Keywords**

NFC, RFAL, SPI, ST25R300

### **Directory contents**

-   app\_x-cube-nfcx.c NFC12 initialization and applicative code

-   analogConfigTbl\_NFC12A1.c Analog configuration file

-   main.c Main program body

-   logger.c Logger file

-   demo\_polling.c NDEF polling File

-   demo\_ce.c manage card emul mode

-   stm32\*\*xx\_hal\_msp.c Source code for MSP Initialization and
    de-Initialization

-   stm32\*\*xx\_it.c Source code for interrupt Service Routines

-   stm32\*\*xx\_nucleo.c Source file for the BSP Common driver

-   stm32\*\*xx\_nucleo\_bus.c Source file for the BSP BUS IO driver

-   system\_stm32\*\*xx.c CMSIS Cortex-Mx Device Peripheral Access Layer
    System Source File

### **Hardware and Software environment**

-   This example runs on STM32 Nucleo devices with NFC12 expansion board
-   X-NUCLEO-NFC12A1

-   This example has been tested with STMicroelectronics:
-   NUCLEO-L476RG RevC board and can be easily tailored to any other
    supported device and development board.

ADDITIONAL\_BOARD : X-NUCLEO-NFC12A1
https://www.st.com/en/ecosystems/x-nucleo-nfc12a1.html ADDITIONAL\_COMP
: ST25R300 https://www.st.com/en/nfc/st25r300.html

### **How to use it?**

In order to make the program work, you must do the following : - Open
your preferred toolchain - Rebuild all files and load your image into
target memory - Run the example - \< Add any other step(s) to be done by
the user to make the example working \>

### **Build & Flash with VS Code / STM32CubeCLT (arm-none-eabi)**

This project can be built and flashed without STM32CubeIDE, using
[STM32CubeCLT](https://www.st.com/en/development-tools/stm32cubeclt.html)
(arm-none-eabi-gcc + Ninja + STM32CubeProgrammer) and VS Code.
The build is driven by `CMakeLists.txt` / `CMakePresets.json`.

**Prerequisites**

- STM32CubeCLT installed (this project was tested with `C:\ST\STM32CubeCLT_1.21.0`).
- NUCLEO-L476RG connected via USB (the on-board ST-LINK is used to flash).

#### VS Code (Tasks)

Open the application folder in VS Code and run a task
(`Ctrl+Shift+P` -> `Tasks: Run Task`):

- **Build (Debug)** - configure + build (output in `build/debug/`).
- **Flash (ST-Link)** - flash the already-built `.elf` to the target.
- **Build & Flash** - build then flash in one step.
- **Clean** - remove build artifacts.

The tasks add the STM32CubeCLT tools to `PATH` automatically (see
`.vscode/tasks.json`); adjust the version path there if your install differs.

#### Command line

```sh
# add STM32CubeCLT tools to PATH (adjust the version if needed)
# CMake\bin, Ninja\bin, GNU-tools-for-STM32\bin, STM32CubeProgrammer\bin

# 1. configure (first time only)
cmake --preset debug

# 2. build  -> build/debug/NFC12A1_PollingTagDetect.elf (+ .hex / .bin)
cmake --build build/debug

# 3. flash via the on-board ST-LINK (SWD), verify and reset
STM32_Programmer_CLI -c port=SWD mode=UR \
    -w build/debug/NFC12A1_PollingTagDetect.elf -v -rst
```

Tip: `STM32_Programmer_CLI -l` lists connected ST-LINK probes and the
virtual COM port (used for the logger UART output).

### **Author**

SRA Application Team

### **License**

Copyright (c) 2022 STMicroelectronics. All rights reserved.

This software is licensed under terms that can be found in the LICENSE
file in the root directory of this software component. If no LICENSE
file comes with this software, it is provided AS-IS.
:::
:::
