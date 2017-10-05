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
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -ffreestanding -fno-builtin -Wall  -g -DDEBUG -DCPU_MK64FN1M0VLL12_cm4 -DCPU_MK64FN1M0VLL12 -DFSL_RTOS_BM -DSDK_CORE_ID_CORE0 -DSDK_OS_BAREMETAL -I"E:\Freescale\SDK_2.2_FRDM-K64F\middleware\usb_1.6.3\include" -I"E:\Freescale\myStrava\libraries" -I"E:\Freescale\myStrava\libraries\sysview" -I"E:\Freescale\SDK_2.2_FRDM-K64F\middleware\sdmmc_2.1.2\inc" -I"E:\Freescale\SDK_2.2_FRDM-K64F\middleware\usb_1.6.3\osa" -I"E:\Freescale\SDK_2.2_FRDM-K64F\middleware\usb_1.6.3\device" -I../CMSIS -I"E:\Freescale\myStrava\source\sd" -I"E:\Freescale\myStrava\source\usb" -I"E:\Freescale\myStrava\source\routes" -I../board -I../drivers -I../startup -I../utilities -std=gnu99 -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


