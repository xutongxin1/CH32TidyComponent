################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
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
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
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
SECONDARY_FLASH += \
adv_vendor.hex \

SECONDARY_LIST += \
adv_vendor.lst \

SECONDARY_SIZE += \
adv_vendor.siz \


# 所有目标
all: adv_vendor.elf secondary-outputs

# 工具调用
adv_vendor.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -T "C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\SRC\Ld\Link.ld" -nostartfiles -Xlinker --gc-sections -L"../" -L"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\MESH\MESH_LIB" -L"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\LIB" -L"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\SRC\StdPeriphDriver" -Xlinker --print-memory-usage -Wl,-Map,"adv_vendor.map" --specs=nano.specs --specs=nosys.specs -o "adv_vendor.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@	@
adv_vendor.hex: adv_vendor.elf
	@	riscv-none-embed-objcopy -O ihex "adv_vendor.elf"  "adv_vendor.hex"
	@	@
adv_vendor.lst: adv_vendor.elf
	@	riscv-none-embed-objdump --source --all-headers --demangle --line-numbers --wide "adv_vendor.elf" > "adv_vendor.lst"
	@	@
adv_vendor.siz: adv_vendor.elf
	@	riscv-none-embed-size --format=berkeley "adv_vendor.elf"
	@	@
# 其他目标
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) adv_vendor.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
