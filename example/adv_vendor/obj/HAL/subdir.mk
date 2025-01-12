################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/xtx/OneDrive\ -\ xutongxin\ (1)/Doc/CH/CH592/EVT/EXAM/BLE/HAL/KEY.c \
C:/Users/xtx/OneDrive\ -\ xutongxin\ (1)/Doc/CH/CH592/EVT/EXAM/BLE/HAL/LED.c \
C:/Users/xtx/OneDrive\ -\ xutongxin\ (1)/Doc/CH/CH592/EVT/EXAM/BLE/HAL/MCU.c \
C:/Users/xtx/OneDrive\ -\ xutongxin\ (1)/Doc/CH/CH592/EVT/EXAM/BLE/HAL/RTC.c \
C:/Users/xtx/OneDrive\ -\ xutongxin\ (1)/Doc/CH/CH592/EVT/EXAM/BLE/HAL/SLEEP.c 

OBJS += \
./HAL/KEY.o \
./HAL/LED.o \
./HAL/MCU.o \
./HAL/RTC.o \
./HAL/SLEEP.o 

C_DEPS += \
./HAL/KEY.d \
./HAL/LED.d \
./HAL/MCU.d \
./HAL/RTC.d \
./HAL/SLEEP.d 


# Each subdirectory must supply rules for building sources it contributes
HAL/KEY.o: C:/Users/xtx/OneDrive\ -\ xutongxin\ (1)/Doc/CH/CH592/EVT/EXAM/BLE/HAL/KEY.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\SRC\Startup" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\MESH\adv_vendor\APP\include" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\MESH\adv_vendor\Profile\include" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\SRC\StdPeriphDriver\inc" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\HAL\include" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\SRC\Ld" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\LIB" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\SRC\RVMSIS" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\MESH\MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
HAL/LED.o: C:/Users/xtx/OneDrive\ -\ xutongxin\ (1)/Doc/CH/CH592/EVT/EXAM/BLE/HAL/LED.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\SRC\Startup" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\MESH\adv_vendor\APP\include" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\MESH\adv_vendor\Profile\include" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\SRC\StdPeriphDriver\inc" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\HAL\include" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\SRC\Ld" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\LIB" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\SRC\RVMSIS" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\MESH\MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
HAL/MCU.o: C:/Users/xtx/OneDrive\ -\ xutongxin\ (1)/Doc/CH/CH592/EVT/EXAM/BLE/HAL/MCU.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\SRC\Startup" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\MESH\adv_vendor\APP\include" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\MESH\adv_vendor\Profile\include" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\SRC\StdPeriphDriver\inc" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\HAL\include" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\SRC\Ld" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\LIB" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\SRC\RVMSIS" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\MESH\MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
HAL/RTC.o: C:/Users/xtx/OneDrive\ -\ xutongxin\ (1)/Doc/CH/CH592/EVT/EXAM/BLE/HAL/RTC.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\SRC\Startup" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\MESH\adv_vendor\APP\include" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\MESH\adv_vendor\Profile\include" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\SRC\StdPeriphDriver\inc" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\HAL\include" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\SRC\Ld" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\LIB" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\SRC\RVMSIS" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\MESH\MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
HAL/SLEEP.o: C:/Users/xtx/OneDrive\ -\ xutongxin\ (1)/Doc/CH/CH592/EVT/EXAM/BLE/HAL/SLEEP.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1 -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\SRC\Startup" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\MESH\adv_vendor\APP\include" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\MESH\adv_vendor\Profile\include" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\SRC\StdPeriphDriver\inc" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\HAL\include" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\SRC\Ld" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\LIB" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\SRC\RVMSIS" -I"C:\Users\xtx\OneDrive - xutongxin (1)\Doc\CH\CH592\EVT\EXAM\BLE\MESH\MESH_LIB" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

