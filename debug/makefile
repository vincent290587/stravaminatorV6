################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utilities/subdir.mk
-include startup/subdir.mk
-include source/usb/subdir.mk
-include source/sd/subdir.mk
-include source/routes/subdir.mk
-include source/power/subdir.mk
-include source/parsers/subdir.mk
-include source/model/subdir.mk
-include source/subdir.mk
-include middleware/usb_1.6.3/osa/subdir.mk
-include middleware/usb_1.6.3/device/subdir.mk
-include middleware/sdmmc_2.1.2/src/subdir.mk
-include middleware/fatfs_0.12b/src/fsl_sd_disk/subdir.mk
-include middleware/fatfs_0.12b/src/subdir.mk
-include middleware/dma_manager_2.1.0/subdir.mk
-include libraries/utils/subdir.mk
-include libraries/sysview/subdir.mk
-include libraries/rtt/subdir.mk
-include libraries/TinyGPSPlus/subdir.mk
-include libraries/TSharpMem/subdir.mk
-include libraries/Nordic/subdir.mk
-include libraries/IntelliScreen/subdir.mk
-include libraries/AdafruitGFX/subdir.mk
-include drivers/uart/subdir.mk
-include drivers/spi/subdir.mk
-include drivers/lcd/subdir.mk
-include drivers/subdir.mk
-include board/subdir.mk
-include CMSIS/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
myStrava.hex \

SECONDARY_SIZE += \
myStrava.siz \


# All Target
all: myStrava.elf secondary-outputs

# Tool invocations
myStrava.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -ffreestanding -fno-builtin -Wall  -g -T "../MK64FN1M0xxx12_flash.ld" -Xlinker --gc-sections -Wl,-Map,"myStrava.map" --specs=nano.specs -mapcs  -Xlinker -static  -Xlinker -z  -Xlinker muldefs -o "myStrava.elf" -Xlinker --start-group $(OBJS) $(USER_OBJS) $(LIBS) -Xlinker --end-group
	@echo 'Finished building target: $@'
	@echo ' '

myStrava.hex: myStrava.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "myStrava.elf"  "myStrava.hex"
	@echo 'Finished building: $@'
	@echo ' '

myStrava.siz: myStrava.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=sysv --totals "myStrava.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(C++_DEPS)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(CC_DEPS)$(SECONDARY_FLASH)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS)$(S_UPPER_DEPS) myStrava.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
