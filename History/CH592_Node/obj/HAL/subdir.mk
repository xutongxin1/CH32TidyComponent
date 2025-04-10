################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
c:/GitProject/CH32TidyComponent/CH592Lib/HAL/KEY.c \
c:/GitProject/CH32TidyComponent/CH592Lib/HAL/LED.c \
c:/GitProject/CH32TidyComponent/CH592Lib/HAL/MCU.c \
c:/GitProject/CH32TidyComponent/CH592Lib/HAL/RTC.c \
c:/GitProject/CH32TidyComponent/CH592Lib/HAL/SLEEP.c 

C_DEPS += \
./HAL/KEY.d \
./HAL/LED.d \
./HAL/MCU.d \
./HAL/RTC.d \
./HAL/SLEEP.d 

OBJS += \
./HAL/KEY.o \
./HAL/LED.o \
./HAL/MCU.o \
./HAL/RTC.o \
./HAL/SLEEP.o 



# Each subdirectory must supply rules for building sources it contributes
HAL/KEY.o: c:/GitProject/CH32TidyComponent/CH592Lib/HAL/KEY.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@
HAL/LED.o: c:/GitProject/CH32TidyComponent/CH592Lib/HAL/LED.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@
HAL/MCU.o: c:/GitProject/CH32TidyComponent/CH592Lib/HAL/MCU.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@
HAL/RTC.o: c:/GitProject/CH32TidyComponent/CH592Lib/HAL/RTC.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@
HAL/SLEEP.o: c:/GitProject/CH32TidyComponent/CH592Lib/HAL/SLEEP.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@

