################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
attiny85_soap.lss \

FLASH_IMAGE += \
attiny85_soap.hex \

EEPROM_IMAGE += \
attiny85_soap.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: attiny85_soap.elf secondary-outputs

# Tool invocations
attiny85_soap.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,attiny85_soap.map -mmcu=attiny85 -o "attiny85_soap.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

attiny85_soap.lss: attiny85_soap.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S attiny85_soap.elf  >"attiny85_soap.lss"
	@echo 'Finished building: $@'
	@echo ' '

attiny85_soap.hex: attiny85_soap.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex attiny85_soap.elf  "attiny85_soap.hex"
	@echo 'Finished building: $@'
	@echo ' '

attiny85_soap.eep: attiny85_soap.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex attiny85_soap.elf  "attiny85_soap.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: attiny85_soap.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=attiny85 attiny85_soap.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) attiny85_soap.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
