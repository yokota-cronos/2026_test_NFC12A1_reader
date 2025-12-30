################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Users/faner/Documents/Repo/NFC12A1+L476/Projects/NUCLEO-L476RG/Applications/NFC12A1_PollingTagDetect/Src/app_x-cube-nfc12.c \
/Users/faner/Documents/Repo/NFC12A1+L476/Projects/NUCLEO-L476RG/Applications/NFC12A1_PollingTagDetect/Src/demo_ce.c \
/Users/faner/Documents/Repo/NFC12A1+L476/Projects/NUCLEO-L476RG/Applications/NFC12A1_PollingTagDetect/Src/demo_polling.c \
/Users/faner/Documents/Repo/NFC12A1+L476/Projects/NUCLEO-L476RG/Applications/NFC12A1_PollingTagDetect/Src/logger.c \
/Users/faner/Documents/Repo/NFC12A1+L476/Projects/NUCLEO-L476RG/Applications/NFC12A1_PollingTagDetect/Src/main.c \
/Users/faner/Documents/Repo/NFC12A1+L476/Projects/NUCLEO-L476RG/Applications/NFC12A1_PollingTagDetect/Src/nfc_conf.c \
/Users/faner/Documents/Repo/NFC12A1+L476/Projects/NUCLEO-L476RG/Applications/NFC12A1_PollingTagDetect/Src/stm32l4xx_hal_msp.c \
/Users/faner/Documents/Repo/NFC12A1+L476/Projects/NUCLEO-L476RG/Applications/NFC12A1_PollingTagDetect/Src/stm32l4xx_it.c \
/Users/faner/Documents/Repo/NFC12A1+L476/Projects/NUCLEO-L476RG/Applications/NFC12A1_PollingTagDetect/Src/stm32l4xx_nucleo.c \
/Users/faner/Documents/Repo/NFC12A1+L476/Projects/NUCLEO-L476RG/Applications/NFC12A1_PollingTagDetect/Src/stm32l4xx_nucleo_bus.c \
../Application/User/syscalls.c \
../Application/User/sysmem.c \
/Users/faner/Documents/Repo/NFC12A1+L476/Projects/NUCLEO-L476RG/Applications/NFC12A1_PollingTagDetect/Src/timer.c 

OBJS += \
./Application/User/app_x-cube-nfc12.o \
./Application/User/demo_ce.o \
./Application/User/demo_polling.o \
./Application/User/logger.o \
./Application/User/main.o \
./Application/User/nfc_conf.o \
./Application/User/stm32l4xx_hal_msp.o \
./Application/User/stm32l4xx_it.o \
./Application/User/stm32l4xx_nucleo.o \
./Application/User/stm32l4xx_nucleo_bus.o \
./Application/User/syscalls.o \
./Application/User/sysmem.o \
./Application/User/timer.o 

C_DEPS += \
./Application/User/app_x-cube-nfc12.d \
./Application/User/demo_ce.d \
./Application/User/demo_polling.d \
./Application/User/logger.d \
./Application/User/main.d \
./Application/User/nfc_conf.d \
./Application/User/stm32l4xx_hal_msp.d \
./Application/User/stm32l4xx_it.d \
./Application/User/stm32l4xx_nucleo.d \
./Application/User/stm32l4xx_nucleo_bus.d \
./Application/User/syscalls.d \
./Application/User/sysmem.d \
./Application/User/timer.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/app_x-cube-nfc12.o: /Users/faner/Documents/Repo/NFC12A1+L476/Projects/NUCLEO-L476RG/Applications/NFC12A1_PollingTagDetect/Src/app_x-cube-nfc12.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DST25R500 -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../../Inc -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/Components/st25r500 -I../../../../../../Middlewares/ST/rfal/Inc -I../../../../../../Middlewares/ST/rfal/Src -I../../../../../../Drivers/BSP/NFC12A1 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/demo_ce.o: /Users/faner/Documents/Repo/NFC12A1+L476/Projects/NUCLEO-L476RG/Applications/NFC12A1_PollingTagDetect/Src/demo_ce.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DST25R500 -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../../Inc -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/Components/st25r500 -I../../../../../../Middlewares/ST/rfal/Inc -I../../../../../../Middlewares/ST/rfal/Src -I../../../../../../Drivers/BSP/NFC12A1 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/demo_polling.o: /Users/faner/Documents/Repo/NFC12A1+L476/Projects/NUCLEO-L476RG/Applications/NFC12A1_PollingTagDetect/Src/demo_polling.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DST25R500 -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../../Inc -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/Components/st25r500 -I../../../../../../Middlewares/ST/rfal/Inc -I../../../../../../Middlewares/ST/rfal/Src -I../../../../../../Drivers/BSP/NFC12A1 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/logger.o: /Users/faner/Documents/Repo/NFC12A1+L476/Projects/NUCLEO-L476RG/Applications/NFC12A1_PollingTagDetect/Src/logger.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DST25R500 -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../../Inc -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/Components/st25r500 -I../../../../../../Middlewares/ST/rfal/Inc -I../../../../../../Middlewares/ST/rfal/Src -I../../../../../../Drivers/BSP/NFC12A1 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/main.o: /Users/faner/Documents/Repo/NFC12A1+L476/Projects/NUCLEO-L476RG/Applications/NFC12A1_PollingTagDetect/Src/main.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DST25R500 -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../../Inc -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/Components/st25r500 -I../../../../../../Middlewares/ST/rfal/Inc -I../../../../../../Middlewares/ST/rfal/Src -I../../../../../../Drivers/BSP/NFC12A1 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/nfc_conf.o: /Users/faner/Documents/Repo/NFC12A1+L476/Projects/NUCLEO-L476RG/Applications/NFC12A1_PollingTagDetect/Src/nfc_conf.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DST25R500 -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../../Inc -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/Components/st25r500 -I../../../../../../Middlewares/ST/rfal/Inc -I../../../../../../Middlewares/ST/rfal/Src -I../../../../../../Drivers/BSP/NFC12A1 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/stm32l4xx_hal_msp.o: /Users/faner/Documents/Repo/NFC12A1+L476/Projects/NUCLEO-L476RG/Applications/NFC12A1_PollingTagDetect/Src/stm32l4xx_hal_msp.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DST25R500 -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../../Inc -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/Components/st25r500 -I../../../../../../Middlewares/ST/rfal/Inc -I../../../../../../Middlewares/ST/rfal/Src -I../../../../../../Drivers/BSP/NFC12A1 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/stm32l4xx_it.o: /Users/faner/Documents/Repo/NFC12A1+L476/Projects/NUCLEO-L476RG/Applications/NFC12A1_PollingTagDetect/Src/stm32l4xx_it.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DST25R500 -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../../Inc -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/Components/st25r500 -I../../../../../../Middlewares/ST/rfal/Inc -I../../../../../../Middlewares/ST/rfal/Src -I../../../../../../Drivers/BSP/NFC12A1 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/stm32l4xx_nucleo.o: /Users/faner/Documents/Repo/NFC12A1+L476/Projects/NUCLEO-L476RG/Applications/NFC12A1_PollingTagDetect/Src/stm32l4xx_nucleo.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DST25R500 -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../../Inc -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/Components/st25r500 -I../../../../../../Middlewares/ST/rfal/Inc -I../../../../../../Middlewares/ST/rfal/Src -I../../../../../../Drivers/BSP/NFC12A1 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/stm32l4xx_nucleo_bus.o: /Users/faner/Documents/Repo/NFC12A1+L476/Projects/NUCLEO-L476RG/Applications/NFC12A1_PollingTagDetect/Src/stm32l4xx_nucleo_bus.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DST25R500 -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../../Inc -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/Components/st25r500 -I../../../../../../Middlewares/ST/rfal/Inc -I../../../../../../Middlewares/ST/rfal/Src -I../../../../../../Drivers/BSP/NFC12A1 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/%.o Application/User/%.su Application/User/%.cyclo: ../Application/User/%.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DST25R500 -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../../Inc -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/Components/st25r500 -I../../../../../../Middlewares/ST/rfal/Inc -I../../../../../../Middlewares/ST/rfal/Src -I../../../../../../Drivers/BSP/NFC12A1 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/timer.o: /Users/faner/Documents/Repo/NFC12A1+L476/Projects/NUCLEO-L476RG/Applications/NFC12A1_PollingTagDetect/Src/timer.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DST25R500 -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../../Inc -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/Components/st25r500 -I../../../../../../Middlewares/ST/rfal/Inc -I../../../../../../Middlewares/ST/rfal/Src -I../../../../../../Drivers/BSP/NFC12A1 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User

clean-Application-2f-User:
	-$(RM) ./Application/User/app_x-cube-nfc12.cyclo ./Application/User/app_x-cube-nfc12.d ./Application/User/app_x-cube-nfc12.o ./Application/User/app_x-cube-nfc12.su ./Application/User/demo_ce.cyclo ./Application/User/demo_ce.d ./Application/User/demo_ce.o ./Application/User/demo_ce.su ./Application/User/demo_polling.cyclo ./Application/User/demo_polling.d ./Application/User/demo_polling.o ./Application/User/demo_polling.su ./Application/User/logger.cyclo ./Application/User/logger.d ./Application/User/logger.o ./Application/User/logger.su ./Application/User/main.cyclo ./Application/User/main.d ./Application/User/main.o ./Application/User/main.su ./Application/User/nfc_conf.cyclo ./Application/User/nfc_conf.d ./Application/User/nfc_conf.o ./Application/User/nfc_conf.su ./Application/User/stm32l4xx_hal_msp.cyclo ./Application/User/stm32l4xx_hal_msp.d ./Application/User/stm32l4xx_hal_msp.o ./Application/User/stm32l4xx_hal_msp.su ./Application/User/stm32l4xx_it.cyclo ./Application/User/stm32l4xx_it.d ./Application/User/stm32l4xx_it.o ./Application/User/stm32l4xx_it.su ./Application/User/stm32l4xx_nucleo.cyclo ./Application/User/stm32l4xx_nucleo.d ./Application/User/stm32l4xx_nucleo.o ./Application/User/stm32l4xx_nucleo.su ./Application/User/stm32l4xx_nucleo_bus.cyclo ./Application/User/stm32l4xx_nucleo_bus.d ./Application/User/stm32l4xx_nucleo_bus.o ./Application/User/stm32l4xx_nucleo_bus.su ./Application/User/syscalls.cyclo ./Application/User/syscalls.d ./Application/User/syscalls.o ./Application/User/syscalls.su ./Application/User/sysmem.cyclo ./Application/User/sysmem.d ./Application/User/sysmem.o ./Application/User/sysmem.su ./Application/User/timer.cyclo ./Application/User/timer.d ./Application/User/timer.o ./Application/User/timer.su

.PHONY: clean-Application-2f-User

