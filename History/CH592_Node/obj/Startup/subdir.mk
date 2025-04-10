################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
c:/GitProject/CH32TidyComponent/CH592Lib/Startup/startup_CH592.S 

S_UPPER_DEPS += \
./Startup/startup_CH592.d 

OBJS += \
./Startup/startup_CH592.o 



# Each subdirectory must supply rules for building sources it contributes
Startup/startup_CH592.o: c:/GitProject/CH32TidyComponent/CH592Lib/Startup/startup_CH592.S
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@

