################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_fxos.c 

OBJS += \
./drivers/fsl_fxos.o 

C_DEPS += \
./drivers/fsl_fxos.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -ffreestanding -fno-builtin -Wall  -g -D_DEBUG=1 -DCPU_MK64FN1M0VLL12_cm4 -DCPU_MK64FN1M0VLL12 -DFSL_RTOS_FREE_RTOS -DFRDM_K64F -DFREEDOM -DUSB_STACK_FREERTOS -DUSB_STACK_FREERTOS_HEAP_SIZE=32768 -I"E:\Freescale\myStrava\libraries\utils" -I"E:\Freescale\myStrava\libraries\sysview" -I"E:\Freescale\myStrava\libraries\rtt" -I"E:\Freescale\myStrava\libraries\AdafruitGFX" -I"E:\Freescale\myStrava\libraries\TSharpMem" -I"E:\Freescale\myStrava\libraries\TinyGPSPlus" -I"E:\Freescale\SDK_2.2_FRDM-K64F\middleware\sdmmc_2.1.2\inc" -I"E:\Freescale\SDK_2.2_FRDM-K64F\middleware\usb_1.6.3\osa" -I"E:\Freescale\SDK_2.2_FRDM-K64F\middleware\usb_1.6.3\device" -I"E:\Freescale\SDK_2.2_FRDM-K64F\middleware\usb_1.6.3\include" -I"E:\Freescale\SDK_2.2_FRDM-K64F\middleware\fatfs_0.12b\src" -I"E:\Freescale\SDK_2.2_FRDM-K64F\middleware\fatfs_0.12b\src\fsl_sd_disk" -I../CMSIS/Include -I../freertos/Source/include -I../freertos/Source/portable/GCC/ARM_CM4F -I../devices/MK64F12 -I../devices/MK64F12/drivers -I../devices/MK64F12/utilities -I"E:\Freescale\myStrava\source\sd" -I"E:\Freescale\myStrava\source\usb" -I"E:\Freescale\myStrava\source\routes" -I"E:\Freescale\myStrava\source\power" -I"E:\Freescale\myStrava\source\parsers" -I"E:\Freescale\myStrava\source\model" -I"E:\Freescale\myStrava\source\display" -I"E:\Freescale\myStrava\source" -I../board -I../drivers -I../startup -I../utilities -I../devices -I"E:\Freescale\myStrava\drivers\lcd" -I"E:\Freescale\myStrava\drivers\spi" -I"E:\Freescale\myStrava\drivers\uart" -std=gnu99 -Wbad-function-cast -MMD -fno-builtin -fno-exceptions -mapcs -nostdlib -larm_cortexM4lf_math -lm -lstdc++ -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


