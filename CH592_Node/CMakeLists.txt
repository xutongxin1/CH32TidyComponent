
cmake_minimum_required(VERSION 3.15)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv)
set(CMAKE_VERBOSE_MAKEFILE ON)

#找编译器，需要自行增加PATH变量
if (0)
    set(CMAKE_C_COMPILER riscv-none-elf-gcc)
    set(CMAKE_CXX_COMPILER riscv-none-elf-g++)
    set(CMAKE_ASM_COMPILER riscv-none-elf-gcc)
    set(CMAKE_LINKER riscv-none-elf-ld)
    set(CMAKE_AR riscv-none-elf-ar)
    set(CMAKE_OBJCOPY riscv-none-elf-objcopy)
    set(CMAKE_OBJDUMP riscv-none-elf-objdump)
    set(SIZE riscv-none-elf-size)
    set(GDB riscv-none-elf-gdb)
else (1)
    set(CMAKE_C_COMPILER riscv-none-embed-gcc)
    set(CMAKE_CXX_COMPILER riscv-none-embed-g++)
    set(CMAKE_ASM_COMPILER riscv-none-embed-gcc)
    set(CMAKE_LINKER riscv-none-embed-ld)
    set(CMAKE_AR riscv-none-embed-ar)
    set(CMAKE_OBJCOPY riscv-none-embed-objcopy)
    set(CMAKE_OBJDUMP riscv-none-embed-objdump)
    set(SIZE riscv-none-embed-size)
    set(GDB riscv-none-embed-gdb)
endif ()
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# set(CMAKE_EXECUTABLE_SUFFIX_C ".elf")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# 设置项目名称
project(CH592_Node LANGUAGES C CXX ASM)


# MCU设置与编译设置
add_compile_options(-march=rv32imac -mabi=ilp32
        -mcmodel=medany -msmall-data-limit=8
        -mno-save-restore -Os -fmessage-length=0
        -fsigned-char -ffunction-sections
        -fdata-sections -fno-common -g -DDEBUG=1
        -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1
        -DCONFIG_I2C_DEBUG=1
)


# 处理ASM文件
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)

#找文件
include_directories(
        "../CH592Lib/HAL/include"
        "../CH592Lib/StdPeriphDriver/inc"
        "../CH592Lib/LIB"
        "../CH592Lib/MESH_LIB"
        "../CH592Lib/RVMSIS"
        "APP/include"
)

file(GLOB_RECURSE SOURCES
        "APP/*.c"
        "../CH592Lib/Startup/startup_CH592.S"
        "../CH592Lib/LIB/ble_task_scheduler.S"
        "../CH592Lib/*.c"
)


set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/../CH592Lib/Ld/LINK.ld)

# 链接设置
add_link_options(-march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8
        -mno-save-restore -Os -fmessage-length=0
        -fsigned-char -ffunction-sections -fdata-sections -fno-common
        -g)
add_link_options(-Wl,--print-memory-usage)
add_link_options(-T ${LINKER_SCRIPT})
add_link_options(-nostartfiles -Xlinker --gc-sections)
add_link_options("-Wl,-Map,${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map" -Wl,--cref)
add_link_options(--specs=nano.specs --specs=nosys.specs -lprintf)
#add_link_options(-lISP592 -lMESH -lCH59xBLE)
#生成elf设置
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

target_link_directories(${PROJECT_NAME}.elf PRIVATE
        ../
        ${CMAKE_SOURCE_DIR}/../CH592Lib/MESH_LIB
        ${CMAKE_SOURCE_DIR}/../CH592Lib/LIB
        ${CMAKE_SOURCE_DIR}/../CH592Lib/StdPeriphDriver
)
target_link_libraries(${PROJECT_NAME}.elf
        ISP592
        MESH
        CH59xBLE
)


set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)
set(LIST_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.list)

#生成hex设置
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${SIZE} --format=berkeley $<TARGET_FILE:${PROJECT_NAME}.elf>
        COMMAND ${CMAKE_OBJDUMP} --source --all-headers --demangle --line-numbers --wide $<TARGET_FILE:${PROJECT_NAME}.elf> > ${LIST_FILE}
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")