################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/system_MK64F12.c 

OBJS += \
./CMSIS/system_MK64F12.o 

C_DEPS += \
./CMSIS/system_MK64F12.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/%.o: ../CMSIS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -ffreestanding -fno-builtin -fsingle-precision-constant -g -Wall -Wshadow -Wlogical-op -DDEBUG -DCPU_MK64FN1M0VLL12_cm4 -DCPU_MK64FN1M0VLL12 -DFSL_RTOS_BM -DSDK_CORE_ID_CORE0 -DSDK_OS_BAREMETAL -I"E:\Freescale\SDK_2.2_FRDM-K64F\middleware\usb_1.6.3\include" -I"E:\Freescale\myStrava\libraries\utils" -I"E:\Freescale\myStrava\libraries\sysview" -I"E:\Freescale\myStrava\libraries\rtt" -I"E:\Freescale\myStrava\libraries\AdafruitGFX" -I"E:\Freescale\myStrava\libraries\Nordic" -I"E:\Freescale\myStrava\libraries\TSharpMem" -I"E:\Freescale\myStrava\libraries\IntelliScreen" -I"E:\Freescale\myStrava\libraries\TinyGPSPlus" -I"E:\Freescale\SDK_2.2_FRDM-K64F\middleware\dma_manager_2.1.0" -I"E:\Freescale\SDK_2.2_FRDM-K64F\middleware\sdmmc_2.1.2\inc" -I"E:\Freescale\SDK_2.2_FRDM-K64F\middleware\usb_1.6.3\osa" -I"E:\Freescale\SDK_2.2_FRDM-K64F\middleware\usb_1.6.3\device" -I"E:\Freescale\SDK_2.2_FRDM-K64F\middleware\fatfs_0.12b\src" -I"E:\Freescale\SDK_2.2_FRDM-K64F\middleware\fatfs_0.12b\src\fsl_sd_disk" -I../CMSIS -I"E:\Freescale\myStrava\source" -I"E:\Freescale\myStrava\source\sd" -I"E:\Freescale\myStrava\source\usb" -I"E:\Freescale\myStrava\source\routes" -I"E:\Freescale\myStrava\source\power" -I"E:\Freescale\myStrava\source\parsers" -I"E:\Freescale\myStrava\source\model" -I"E:\Freescale\myStrava\source\display" -I"E:\Freescale\myStrava\source\sensors" -I../board -I../drivers -I../startup -I../utilities -I"E:\Freescale\myStrava\drivers\lcd" -I"E:\Freescale\myStrava\drivers\spi" -I"E:\Freescale\myStrava\drivers\uart" -I"E:\Freescale\myStrava\drivers\i2c" -I"E:\Freescale\myStrava\drivers\xfer" -std=gnu99 -Wbad-function-cast -MMD -fno-builtin -fno-exceptions -mapcs -nostdlib -larm_cortexM4lf_math -lm -lstdc++ -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


