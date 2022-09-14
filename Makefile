PROJECT 		?= app
# path to gcc arm (or should be specified in PATH)
ARM_TOOCHAIN 	?= /opt/gcc-arm/gcc-arm-11.2-2022.02-x86_64-arm-none-eabi/bin
# path to JLinkExe (or should be specified in PATH)
JLINKEXE		?= /opt/SEGGER/JLink/JLinkExe
# MCU type: HK32F030MD4P6, HK32F030MF4U6, HK32F030MF4P6 or HK32F030MJ4M6
DEVICE			?= HK32F030MF4P6
# The path for generated files
BUILD_DIR 		= Build

# Link descript file
LDSCRIPT		= Libraries/LDScripts/hk32f030mf4p6_lite.ld
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
AFILES	:=

# Include paths
INCLUDES	:= Libraries/CMSIS/CM0/Core \
			Libraries/CMSIS/HK32F030M/Include \
			Libraries/HK32F030M_Driver/inc \
			Libraries/Debug \
			User

include ./rules.mk
