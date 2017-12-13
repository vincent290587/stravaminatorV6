################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_clock.c \
../drivers/fsl_cmt.c \
../drivers/fsl_common.c \
../drivers/fsl_crc.c \
../drivers/fsl_dmamux.c \
../drivers/fsl_dspi.c \
../drivers/fsl_dspi_edma.c \
../drivers/fsl_edma.c \
../drivers/fsl_flash.c \
../drivers/fsl_flexbus.c \
../drivers/fsl_ftm.c \
../drivers/fsl_fxos.c \
../drivers/fsl_gpio.c \
../drivers/fsl_i2c.c \
../drivers/fsl_i2c_cmsis.c \
../drivers/fsl_i2c_edma.c \
../drivers/fsl_llwu.c \
../drivers/fsl_lptmr.c \
../drivers/fsl_pdb.c \
../drivers/fsl_pit.c \
../drivers/fsl_pmc.c \
../drivers/fsl_rcm.c \
../drivers/fsl_rnga.c \
../drivers/fsl_rtc.c \
../drivers/fsl_sai.c \
../drivers/fsl_sai_edma.c \
../drivers/fsl_sdhc.c \
../drivers/fsl_sim.c \
../drivers/fsl_smc.c \
../drivers/fsl_sysmpu.c \
../drivers/fsl_uart.c \
../drivers/fsl_uart_edma.c \
../drivers/fsl_wdog.c 

OBJS += \
./drivers/fsl_clock.o \
./drivers/fsl_cmt.o \
./drivers/fsl_common.o \
./drivers/fsl_crc.o \
./drivers/fsl_dmamux.o \
./drivers/fsl_dspi.o \
./drivers/fsl_dspi_edma.o \
./drivers/fsl_edma.o \
./drivers/fsl_flash.o \
./drivers/fsl_flexbus.o \
./drivers/fsl_ftm.o \
./drivers/fsl_fxos.o \
./drivers/fsl_gpio.o \
./drivers/fsl_i2c.o \
./drivers/fsl_i2c_cmsis.o \
./drivers/fsl_i2c_edma.o \
./drivers/fsl_llwu.o \
./drivers/fsl_lptmr.o \
./drivers/fsl_pdb.o \
./drivers/fsl_pit.o \
./drivers/fsl_pmc.o \
./drivers/fsl_rcm.o \
./drivers/fsl_rnga.o \
./drivers/fsl_rtc.o \
./drivers/fsl_sai.o \
./drivers/fsl_sai_edma.o \
./drivers/fsl_sdhc.o \
./drivers/fsl_sim.o \
./drivers/fsl_smc.o \
./drivers/fsl_sysmpu.o \
./drivers/fsl_uart.o \
./drivers/fsl_uart_edma.o \
./drivers/fsl_wdog.o 

C_DEPS += \
./drivers/fsl_clock.d \
./drivers/fsl_cmt.d \
./drivers/fsl_common.d \
./drivers/fsl_crc.d \
./drivers/fsl_dmamux.d \
./drivers/fsl_dspi.d \
./drivers/fsl_dspi_edma.d \
./drivers/fsl_edma.d \
./drivers/fsl_flash.d \
./drivers/fsl_flexbus.d \
./drivers/fsl_ftm.d \
./drivers/fsl_fxos.d \
./drivers/fsl_gpio.d \
./drivers/fsl_i2c.d \
./drivers/fsl_i2c_cmsis.d \
./drivers/fsl_i2c_edma.d \
./drivers/fsl_llwu.d \
./drivers/fsl_lptmr.d \
./drivers/fsl_pdb.d \
./drivers/fsl_pit.d \
./drivers/fsl_pmc.d \
./drivers/fsl_rcm.d \
./drivers/fsl_rnga.d \
./drivers/fsl_rtc.d \
./drivers/fsl_sai.d \
./drivers/fsl_sai_edma.d \
./drivers/fsl_sdhc.d \
./drivers/fsl_sim.d \
./drivers/fsl_smc.d \
./drivers/fsl_sysmpu.d \
./drivers/fsl_uart.d \
./drivers/fsl_uart_edma.d \
./drivers/fsl_wdog.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -ffreestanding -fno-builtin -fsingle-precision-constant -g -Wall -Wshadow -Wlogical-op -DDEBUG -DCPU_MK64FN1M0VLL12_cm4 -DCPU_MK64FN1M0VLL12 -DFSL_RTOS_BM -DSDK_CORE_ID_CORE0 -DSDK_OS_BAREMETAL -I"E:\Freescale\SDK_2.3.0_FRDM-K64F\middleware\usb\include" -I"E:\Freescale\myStrava\libraries\utils" -I"E:\Freescale\myStrava\libraries\sysview" -I"E:\Freescale\myStrava\libraries\rtt" -I"E:\Freescale\myStrava\libraries\AdafruitGFX" -I"E:\Freescale\myStrava\libraries\Nordic" -I"E:\Freescale\myStrava\libraries\TSharpMem" -I"E:\Freescale\myStrava\libraries\IntelliScreen" -I"E:\Freescale\myStrava\libraries\TinyGPSPlus" -I"E:\Freescale\SDK_2.3.0_FRDM-K64F\middleware\dma_manager" -I"E:\Freescale\SDK_2.3.0_FRDM-K64F\middleware\sdmmc\inc" -I"E:\Freescale\SDK_2.3.0_FRDM-K64F\middleware\usb\osa" -I"E:\Freescale\SDK_2.3.0_FRDM-K64F\middleware\usb\device" -I"E:\Freescale\SDK_2.3.0_FRDM-K64F\middleware\fatfs\src" -I"E:\Freescale\SDK_2.3.0_FRDM-K64F\middleware\fatfs\src\fsl_sd_disk" -I../CMSIS -I"E:\Freescale\myStrava\source" -I"E:\Freescale\myStrava\source\sd" -I"E:\Freescale\myStrava\source\usb" -I"E:\Freescale\myStrava\source\routes" -I"E:\Freescale\myStrava\source\power" -I"E:\Freescale\myStrava\source\parsers" -I"E:\Freescale\myStrava\source\model" -I"E:\Freescale\myStrava\source\display" -I"E:\Freescale\myStrava\source\sensors" -I"E:\Freescale\myStrava\source\scheduling" -I"E:\Freescale\myStrava\source\vue" -I../board -I../drivers -I../startup -I../utilities -I"E:\Freescale\myStrava\drivers\lcd" -I"E:\Freescale\myStrava\drivers\spi" -I"E:\Freescale\myStrava\drivers\uart" -I"E:\Freescale\myStrava\drivers\i2c" -std=gnu99 -Wbad-function-cast -MMD -fno-builtin -fno-exceptions -mapcs -nostdlib -larm_cortexM4lf_math -lm -lstdc++ -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


