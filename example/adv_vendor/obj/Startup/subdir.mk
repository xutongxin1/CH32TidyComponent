################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
C:/Users/xtx/OneDrive\ -\ xutongxin\ (1)/Doc/CH/CH592/EVT/EXAM/SRC/Startup/startup_CH592.S 

OBJS += \
./Startup/startup_CH592.o 

S_UPPER_DEPS += \
./Startup/startup_CH592.d 


# Each subdirectory must supply rules for building sources it contributes
Startup/startup_CH592.o: C:/Users/xtx/OneDrive\ -\ xutongxin\ (1)/Doc/CH/CH592/EVT/EXAM/SRC/Startup/startup_CH592.S
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

