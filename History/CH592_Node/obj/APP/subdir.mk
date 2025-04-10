################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../APP/app.c \
../APP/app_main.c \
../APP/app_mesh_config.c \
../APP/app_vendor_model_srv.c 

C_DEPS += \
./APP/app.d \
./APP/app_main.d \
./APP/app_mesh_config.d \
./APP/app_vendor_model_srv.d 

OBJS += \
./APP/app.o \
./APP/app_main.o \
./APP/app_mesh_config.o \
./APP/app_vendor_model_srv.o 



# Each subdirectory must supply rules for building sources it contributes
APP/%.o: ../APP/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"c:/GitProject/CH32TidyComponent/CH592Lib/Startup" -I"c:/GitProject/CH32TidyComponent/CH592_Node/APP/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/StdPeriphDriver/inc" -I"c:/GitProject/CH32TidyComponent/CH592Lib/HAL/include" -I"c:/GitProject/CH32TidyComponent/CH592Lib/Ld" -I"c:/GitProject/CH32TidyComponent/CH592Lib/LIB" -I"c:/GitProject/CH32TidyComponent/CH592Lib/RVMSIS" -I"c:/GitProject/CH32TidyComponent/CH592Lib/MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@
