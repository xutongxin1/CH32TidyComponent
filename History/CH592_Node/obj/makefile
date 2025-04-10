################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################
-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include StdPeriphDriver/subdir.mk
-include Startup/subdir.mk
-include RVMSIS/subdir.mk
-include LIB/subdir.mk
-include HAL/subdir.mk
-include APP/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
CH592_Node.hex \

SECONDARY_LIST += \
CH592_Node.lst \

SECONDARY_SIZE += \
CH592_Node.siz \


# All Target
all: 
	$(MAKE) --no-print-directory main-build 

main-build: CH592_Node.elf secondary-outputs

# Tool invocations
CH592_Node.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU RISC-V Cross C Linker'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -T "c:/GitProject/CH32TidyComponent/CH592Lib/Ld/Link.ld" -nostartfiles -Xlinker --gc-sections -L"c:\GitProject\CH32TidyComponent\CH592_Node" -L"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -L"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -L"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver" -Xlinker --print-memory-usage -Xlinker --print-memory-usage -Wl,-Map,"CH592_Node.map" --specs=nano.specs --specs=nosys.specs -o "CH592_Node.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@
CH592_Node.hex: CH592_Node.elf
	@echo 'Invoking: GNU RISC-V Cross Create Flash Image'
	@	riscv-none-embed-objcopy -O ihex "CH592_Node.elf" "CH592_Node.hex"
	@echo 'Finished building $@'
	@
CH592_Node.lst: CH592_Node.elf
	@echo 'Invoking: GNU RISC-V Cross Create Listing'
	@	riscv-none-embed-objdump --source --all-headers --demangle --line-numbers --wide "CH592_Node.elf" > "CH592_Node.lst"
	@echo 'Finished building $@'
	@
CH592_Node.siz: CH592_Node.elf
	@echo 'Invoking: GNU RISC-V Cross Print Size'
	riscv-none-embed-size --format=berkeley "CH592_Node.elf"
	@echo 'Finished building $@'
	@

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(S_DEPS)$(S_UPPER_DEPS)$(ASM_DEPS)$(ASM_UPPER_DEPS)$(C_DEPS) CH592_Node.elf

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets