cmake_minimum_required(VERSION 3.22)

# ---------------------------------------------------------------------------
# NFC12A1_PollingTagDetect  -  NUCLEO-L476RG (STM32L476RGTx, Cortex-M4F)
# CMake build mirroring the original STM32CubeIDE project, for use from
# VS Code + STM32CubeCLT (arm-none-eabi-gcc / STM32CubeProgrammer).
# ---------------------------------------------------------------------------

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

project(NFC12A1_PollingTagDetect C ASM)

# Repository root (this app sits 4 levels below it).
get_filename_component(REPO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../.." ABSOLUTE)

# Cortex-M4 with hardware single-precision FPU.
set(MCU_FLAGS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard)

# ---- Sources --------------------------------------------------------------
file(GLOB APP_SRC  "${CMAKE_CURRENT_SOURCE_DIR}/Src/*.c")
file(GLOB HAL_SRC  "${REPO_ROOT}/Drivers/STM32L4xx_HAL_Driver/Src/*.c")
file(GLOB BSP_SRC  "${REPO_ROOT}/Drivers/BSP/Components/st25r500/*.c")
file(GLOB RFAL_SRC "${REPO_ROOT}/Middlewares/ST/rfal/Src/*.c")

# HAL *_template.c files are reference samples, not meant to be compiled.
list(FILTER HAL_SRC EXCLUDE REGEX "_template\\.c$")

# rfal_dlma.c is not part of the original X-CUBE-NFC12 build set.
list(FILTER RFAL_SRC EXCLUDE REGEX "rfal_dlma\\.c$")

set(SYSCALLS
    "${CMAKE_CURRENT_SOURCE_DIR}/STM32CubeIDE/Application/User/syscalls.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/STM32CubeIDE/Application/User/sysmem.c")

set(STARTUP     "${CMAKE_CURRENT_SOURCE_DIR}/STM32CubeIDE/Application/Startup/startup_stm32l476rgtx.s")
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/STM32CubeIDE/STM32L476RGTX_FLASH.ld")

add_executable(${PROJECT_NAME}.elf
    ${APP_SRC}
    ${HAL_SRC}
    ${BSP_SRC}
    ${RFAL_SRC}
    ${SYSCALLS}
    ${STARTUP})

# ---- Defines --------------------------------------------------------------
target_compile_definitions(${PROJECT_NAME}.elf PRIVATE
    ST25R500
    USE_HAL_DRIVER
    STM32L476xx)

# ---- Includes -------------------------------------------------------------
target_include_directories(${PROJECT_NAME}.elf PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Inc"
    "${REPO_ROOT}/Drivers/STM32L4xx_HAL_Driver/Inc"
    "${REPO_ROOT}/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy"
    "${REPO_ROOT}/Drivers/CMSIS/Device/ST/STM32L4xx/Include"
    "${REPO_ROOT}/Drivers/CMSIS/Include"
    "${REPO_ROOT}/Drivers/BSP/Components/st25r500"
    "${REPO_ROOT}/Drivers/BSP/NFC12A1"
    "${REPO_ROOT}/Middlewares/ST/rfal/Inc"
    "${REPO_ROOT}/Middlewares/ST/rfal/Src")

# ---- Compile options ------------------------------------------------------
target_compile_options(${PROJECT_NAME}.elf PRIVATE
    ${MCU_FLAGS}
    -ffunction-sections -fdata-sections -fno-common
    -Wall
    $<$<CONFIG:Debug>:-Og -g3 -gdwarf-2>
    $<$<CONFIG:Release>:-Os -g0>
    $<$<CONFIG:RelWithDebInfo>:-Os -g3>)

# ---- Link options ---------------------------------------------------------
target_link_options(${PROJECT_NAME}.elf PRIVATE
    ${MCU_FLAGS}
    -Wl,-T,${LINKER_SCRIPT}
    --specs=nano.specs
    -Wl,-Map=${PROJECT_NAME}.map,--cref
    -Wl,--gc-sections
    -Wl,--start-group -lc -lm -Wl,--end-group)

# ---- Post-build: print size, generate .hex and .bin -----------------------
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND "${CMAKE_SIZE}" "$<TARGET_FILE:${PROJECT_NAME}.elf>"
    COMMAND "${CMAKE_OBJCOPY}" -O ihex   "$<TARGET_FILE:${PROJECT_NAME}.elf>" "${PROJECT_NAME}.hex"
    COMMAND "${CMAKE_OBJCOPY}" -O binary "$<TARGET_FILE:${PROJECT_NAME}.elf>" "${PROJECT_NAME}.bin"
    COMMENT "Generating .hex / .bin and size report")
