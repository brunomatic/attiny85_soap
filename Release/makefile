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
attiny_proto.lss \

FLASH_IMAGE += \
attiny_proto.hex \

EEPROM_IMAGE += \
attiny_proto.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: attiny_proto.elf secondary-outputs

# Tool invocations
attiny_proto.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,attiny_proto.map -mmcu=attiny85 -o "attiny_proto.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

attiny_proto.lss: attiny_proto.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S attiny_proto.elf  >"attiny_proto.lss"
	@echo 'Finished building: $@'
	@echo ' '

attiny_proto.hex: attiny_proto.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex attiny_proto.elf  "attiny_proto.hex"
	@echo 'Finished building: $@'
	@echo ' '

attiny_proto.eep: attiny_proto.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex attiny_proto.elf  "attiny_proto.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: attiny_proto.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=attiny85 attiny_proto.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) attiny_proto.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
