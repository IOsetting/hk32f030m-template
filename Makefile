##### Project #####

PROJECT 		?= app
# The path for generated files
BUILD_DIR 		= Build


##### Options #####

# Enable printf float %f support, y:yes, n:no
ENABLE_PRINTF_FLOAT	?= n
# Build with FreeRTOS, y:yes, n:no
USE_FREERTOS	?= n
# Programmer, jlink or pyocd
FLASH_PROGRM	?= jlink


##### Toolchains #######

# path to gcc arm
#ARM_TOOCHAIN 	?= /opt/gcc-arm/gcc-arm-11.2-2022.02-x86_64-arm-none-eabi/bin
#ARM_TOOCHAIN 	?= /opt/gcc-arm/arm-gnu-toolchain-11.3.rel1-x86_64-arm-none-eabi/bin
ARM_TOOCHAIN 	?= /opt/gcc-arm/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin

# path to JLinkExe
JLINKEXE		?= /opt/SEGGER/JLink/JLinkExe
# JLink device type: HK32F030MD4P6, HK32F030MF4U6, HK32F030MF4P6 or HK32F030MJ4M6
JLINK_DEVICE	?= HK32F030MF4P6
# path to PyOCD
PYOCD_EXE		?= pyocd
# PyOCD device type: hk32f030md4p6, hk32f030mf4p6, hk32f030mf4u6, hk32f030mj4m6
PYOCD_DEVICE	?= hk32f030mf4p6


##### Paths ############

# Link descript file
LDSCRIPT		= Libraries/LDScripts/hk32f030mf4p6.ld
# Library build flags
LIB_FLAGS       = HK32F030MF4P6

# C source folders
CDIRS	:= User \
		Libraries/CMSIS/HK32F030M/Source \
		Libraries/HK32F030M_Driver/src \
		Libraries/Debug
# C source files (if there are any single ones)
CFILES := 

# ASM source folders
ADIRS	:= User
# ASM single files
AFILES	:= Libraries/CMSIS/HK32F030M/Source/startup_hk32f030mf4p6.s

# Include paths
INCLUDES	:= User \
			Libraries/CMSIS/Core/Include \
			Libraries/CMSIS/HK32F030M/Include \
			Libraries/HK32F030M_Driver/inc \
			Libraries/Debug

ifeq ($(USE_FREERTOS),y)
CDIRS		+= Libraries/FreeRTOS \
			Libraries/FreeRTOS/portable/GCC/ARM_CM0

CFILES		+= Libraries/FreeRTOS/portable/MemMang/heap_4.c

INCLUDES	+= Libraries/FreeRTOS/include \
			Libraries/FreeRTOS/portable/GCC/ARM_CM0
endif

include ./rules.mk
