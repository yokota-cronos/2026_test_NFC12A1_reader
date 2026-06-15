# ARM bare-metal toolchain file (STM32CubeCLT GNU tools)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Path to the GNU arm toolchain shipped with STM32CubeCLT.
# Override with -DTOOLCHAIN_BIN=... if your CLT version/path differs.
if(NOT DEFINED TOOLCHAIN_BIN)
    set(TOOLCHAIN_BIN "C:/ST/STM32CubeCLT_1.21.0/GNU-tools-for-STM32/bin")
endif()

set(TOOLCHAIN_PREFIX "${TOOLCHAIN_BIN}/arm-none-eabi-")

set(CMAKE_C_COMPILER   "${TOOLCHAIN_PREFIX}gcc.exe")
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_PREFIX}gcc.exe")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}g++.exe")
set(CMAKE_OBJCOPY      "${TOOLCHAIN_PREFIX}objcopy.exe" CACHE FILEPATH "")
set(CMAKE_SIZE         "${TOOLCHAIN_PREFIX}size.exe"    CACHE FILEPATH "")

# Don't try to build a full test executable during compiler check (no syscalls yet).
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
