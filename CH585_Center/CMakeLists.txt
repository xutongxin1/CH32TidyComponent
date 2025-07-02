
cmake_minimum_required(VERSION 3.15)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv)

#找编译器，需要自行增加PATH变量
if (1)
    set(CMAKE_C_COMPILER riscv-wch-elf-gcc)
    set(CMAKE_CXX_COMPILER riscv-wch-elf-g++)
    set(CMAKE_ASM_COMPILER riscv-wch-elf-gcc)
    set(CMAKE_LINKER riscv-wch-elf-ld)
    set(CMAKE_AR riscv-wch-elf-ar)
    set(CMAKE_OBJCOPY riscv-wch-elf-objcopy)
    set(CMAKE_OBJDUMP riscv-wch-elf-objdump)
    set(SIZE riscv-wch-elf-size)
    set(GDB riscv-wch-elf-gdb)
else (0)
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
project(CH585_Center LANGUAGES C CXX ASM)


# MCU设置与编译设置
add_compile_options(-march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32)

add_compile_options(
        -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os
        -fmessage-length=0 -fsigned-char -ffunction-sections
        -fdata-sections -fno-common --param=highcode-gen-section-name=1
        -g -DDEBUG=0 -DBLE_MEMHEAP_SIZE=4096 -DHAL_KEY=1
        -std=gnu99 -MMD -MP
)

# 处理ASM文件
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)


#找文件
include_directories(
        "../CH585Lib/HAL/include"
        "../CH585Lib/StdPeriphDriver/inc"
        "../CH585Lib/LIB"
        "../CH585Lib/MESH_LIB"
        "../CH585Lib/RVMSIS"
        "APP/include"
        "Lib/MIFARECLASSIC_COMMON"
        "Lib/NFCA_COMMON"
        "Lib/NFCA_PCD_LIB"
)

file(GLOB_RECURSE SOURCES
        "APP/*.c"
        "Lib/*.c"
        "../CH585Lib/Startup/startup_CH585.S"
        "../CH585Lib/LIB/ble_task_scheduler.S"
        "../CH585Lib/*.c"
)


set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/../CH585Lib/Ld/LINK.ld)

# 链接设置
add_link_options(-march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32)
add_link_options(
        -mcmodel=medany -msmall-data-limit=8 -mno-save-restore
        -Os -fmessage-length=0 -fsigned-char -ffunction-sections
        -fdata-sections -fno-common --param=highcode-gen-section-name=1
        -g
)
add_link_options(-Wl,--print-memory-usage)
add_link_options(-T ${LINKER_SCRIPT})
add_link_options(-nostartfiles -Xlinker --gc-sections)
add_link_options("-Wl,-Map,${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map" -Wl,--cref)
add_link_options(--specs=nano.specs --specs=nosys.specs -lprintfloat)

#生成elf设置
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

target_link_directories(${PROJECT_NAME}.elf PRIVATE
        ../
        ${CMAKE_SOURCE_DIR}/../CH585Lib/MESH_LIB
        ${CMAKE_SOURCE_DIR}/../CH585Lib/LIB
        ${CMAKE_SOURCE_DIR}/../CH585Lib/StdPeriphDriver
        ${CMAKE_SOURCE_DIR}/Lib/NFCA_COMMON
        ${CMAKE_SOURCE_DIR}/Lib/NFCA_PCD_LIB
)
target_link_libraries(${PROJECT_NAME}.elf
        ISP585
        MESH
        CH58xBLE
        libwch_nfca_pcd.a
        libwch_nfca_crypto1.a
)

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)
set(LIST_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.list)

#生成hex设置
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${SIZE} --format=berkeley $<TARGET_FILE:${PROJECT_NAME}.elf>
        COMMAND ${CMAKE_OBJDUMP} --source --all-headers --demangle --line-numbers --wide -M xw $<TARGET_FILE:${PROJECT_NAME}.elf> > ${LIST_FILE}
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")

# 添加自定义目标 "flash"
add_custom_target(Erase_Flash
        COMMAND ../wlink.exe erase
        COMMAND ../wlink.exe flash ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.elf
        DEPENDS ${PROJECT_NAME}.elf
        WORKING_DIRECTORY "C:/GitProject/CH32TidyComponent/CH585_Center"
        COMMENT "执行擦除和烧录操作"
        VERBATIM
        USES_TERMINAL
)

add_custom_target(Flash
        COMMAND ../wlink.exe flash ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.elf
        DEPENDS ${PROJECT_NAME}.elf
        WORKING_DIRECTORY "C:/GitProject/CH32TidyComponent/CH585_Center"
        COMMENT "执行擦除和烧录操作"
        VERBATIM
        USES_TERMINAL
)

add_custom_target(Reset
        COMMAND ../wlink.exe reset
        WORKING_DIRECTORY "C:/GitProject/CH32TidyComponent/CH585_Center"
        COMMENT "复位"
        VERBATIM
        USES_TERMINAL
)