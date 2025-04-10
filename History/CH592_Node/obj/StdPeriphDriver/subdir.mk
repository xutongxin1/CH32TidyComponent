################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_adc.c \
c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_clk.c \
c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_flash.c \
c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_gpio.c \
c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_i2c.c \
c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_lcd.c \
c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_pwm.c \
c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_pwr.c \
c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_spi0.c \
c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_sys.c \
c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_timer0.c \
c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_timer1.c \
c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_timer2.c \
c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_timer3.c \
c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_uart0.c \
c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_uart1.c \
c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_uart2.c \
c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_uart3.c \
c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_usbdev.c \
c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_usbhostBase.c \
c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_usbhostClass.c 

C_DEPS += \
./StdPeriphDriver/CH59x_adc.d \
./StdPeriphDriver/CH59x_clk.d \
./StdPeriphDriver/CH59x_flash.d \
./StdPeriphDriver/CH59x_gpio.d \
./StdPeriphDriver/CH59x_i2c.d \
./StdPeriphDriver/CH59x_lcd.d \
./StdPeriphDriver/CH59x_pwm.d \
./StdPeriphDriver/CH59x_pwr.d \
./StdPeriphDriver/CH59x_spi0.d \
./StdPeriphDriver/CH59x_sys.d \
./StdPeriphDriver/CH59x_timer0.d \
./StdPeriphDriver/CH59x_timer1.d \
./StdPeriphDriver/CH59x_timer2.d \
./StdPeriphDriver/CH59x_timer3.d \
./StdPeriphDriver/CH59x_uart0.d \
./StdPeriphDriver/CH59x_uart1.d \
./StdPeriphDriver/CH59x_uart2.d \
./StdPeriphDriver/CH59x_uart3.d \
./StdPeriphDriver/CH59x_usbdev.d \
./StdPeriphDriver/CH59x_usbhostBase.d \
./StdPeriphDriver/CH59x_usbhostClass.d 

OBJS += \
./StdPeriphDriver/CH59x_adc.o \
./StdPeriphDriver/CH59x_clk.o \
./StdPeriphDriver/CH59x_flash.o \
./StdPeriphDriver/CH59x_gpio.o \
./StdPeriphDriver/CH59x_i2c.o \
./StdPeriphDriver/CH59x_lcd.o \
./StdPeriphDriver/CH59x_pwm.o \
./StdPeriphDriver/CH59x_pwr.o \
./StdPeriphDriver/CH59x_spi0.o \
./StdPeriphDriver/CH59x_sys.o \
./StdPeriphDriver/CH59x_timer0.o \
./StdPeriphDriver/CH59x_timer1.o \
./StdPeriphDriver/CH59x_timer2.o \
./StdPeriphDriver/CH59x_timer3.o \
./StdPeriphDriver/CH59x_uart0.o \
./StdPeriphDriver/CH59x_uart1.o \
./StdPeriphDriver/CH59x_uart2.o \
./StdPeriphDriver/CH59x_uart3.o \
./StdPeriphDriver/CH59x_usbdev.o \
./StdPeriphDriver/CH59x_usbhostBase.o \
./StdPeriphDriver/CH59x_usbhostClass.o 



# Each subdirectory must supply rules for building sources it contributes
StdPeriphDriver/CH59x_adc.o: c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_adc.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@
StdPeriphDriver/CH59x_clk.o: c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_clk.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@
StdPeriphDriver/CH59x_flash.o: c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_flash.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@
StdPeriphDriver/CH59x_gpio.o: c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_gpio.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@
StdPeriphDriver/CH59x_i2c.o: c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_i2c.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@
StdPeriphDriver/CH59x_lcd.o: c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_lcd.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@
StdPeriphDriver/CH59x_pwm.o: c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_pwm.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@
StdPeriphDriver/CH59x_pwr.o: c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_pwr.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@
StdPeriphDriver/CH59x_spi0.o: c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_spi0.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@
StdPeriphDriver/CH59x_sys.o: c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_sys.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@
StdPeriphDriver/CH59x_timer0.o: c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_timer0.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@
StdPeriphDriver/CH59x_timer1.o: c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_timer1.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@
StdPeriphDriver/CH59x_timer2.o: c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_timer2.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@
StdPeriphDriver/CH59x_timer3.o: c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_timer3.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@
StdPeriphDriver/CH59x_uart0.o: c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_uart0.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@
StdPeriphDriver/CH59x_uart1.o: c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_uart1.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@
StdPeriphDriver/CH59x_uart2.o: c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_uart2.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@
StdPeriphDriver/CH59x_uart3.o: c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_uart3.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@
StdPeriphDriver/CH59x_usbdev.o: c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_usbdev.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@
StdPeriphDriver/CH59x_usbhostBase.o: c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_usbhostBase.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@
StdPeriphDriver/CH59x_usbhostClass.o: c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/CH59x_usbhostClass.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@

