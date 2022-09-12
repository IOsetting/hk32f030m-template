# HK32F030MF4P6 Project Template 

A build template for projects using HK32F030MF4P6 MCU and GNU Arm Embedded Toolchain.

# HK32F030MF4P6

## Pinout - TSSOP20

| Pin | Func          | Pin | Func             |
| -- | -------------- | -- | ----------------- |
| 1  | PD4            | 20 | PD3 AIN3          |  
| 2  | PD5 AIN0 SWDIO | 19 | PD2 AIN4          |  
| 3  | PD6 AIN1       | 18 | PD1               |  
| 4  | NRST           | 17 | PC7               |  
| 5  | PA1 EXTCLK1    | 16 | PC6               |  
| 6  | PA2            | 15 | PC5 EXTCLK4       |  
| 7  | VSS VSSA       | 14 | PC4 AIN2 WKUP0    |  
| 8  | PD7 VCAP       | 13 | PC3               |  
| 9  | VDD VDDA       | 12 | PB4               |  
| 10 | PA3            | 11 | PB5 EXTCLK3 SWCLK |  

# File Structure

```
├── Build                       # Build results
├── Examples                    # Example code
├── flash.jlink                 # JLink download commander script
├── Libraries
│   ├── CMSIS
│   ├── Debug                   # SysTick delay and printf for debug
│   ├── HK32F030M_Driver        # MCU peripheral driver
│   └── LDScripts
├── LICENSE
├── Makefile                    # Make config
├── README.md
├── rules.mk                    # Pre-defined rules include in Makefile 
└── User                        # User application code
```

# Requirements

* Board using HK32F030MF4P6
* JLink OB programmer
* SEGGER J-Link Software and Documentation pack [https://www.segger.com/downloads/jlink/](https://www.segger.com/downloads/jlink/)
* GNU Arm Embedded Toolchain
* HK32F030MF4P6_Fwlib-Template.zip download from
  * [https://bbs.21ic.com/icview-3236132-1-1.html](https://bbs.21ic.com/icview-3236132-1-1.html)


# Building

## 1. Install GNU Arm Embedded Toolchain

Download the toolchain from [Arm GNU Toolchain Downloads](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) according to your pc architecture, extract the files

```bash
tar xvf gcc-arm-11.2-2022.02-x86_64-arm-none-eabi.tar.xz
cd /opt/gcc-arm/
sudo mv ~/Backup/linux/gcc-arm-11.2-2022.02-x86_64-arm-none-eabi/ .
sudo chown -R root:root gcc-arm-11.2-2022.02-x86_64-arm-none-eabi/
```
## 2. Install SEGGER J-Link

Download and install JLink from [J-Link / J-Trace Downloads](https://www.segger.com/downloads/jlink/).

```bash
# installation command for .deb
sudo dpkg -i JLink_Linux_V770a_x86_64.deb
```
The default installation directory is */opt/SEGGER*

## 3. Clone This Repository

Clone this repository to local workspace
```bash
git clone https://github.com/IOsetting/hk32f030m-template.git
```
Uncompress HK32F030MF4P6_Fwlib-Template.zip and copy files under Source/Libraries/HK32F030M_Lib/ to 
Libraries/HK32F030M_Driver, make the file structure looks like
```
├── Build
├── Examples
├── flash.jlink
├── Libraries
│   ├── CMSIS
│   ├── Debug
│   ├── HK32F030M_Driver
│   │   ├── inc
│   │   │   ├── hk32f030m_adc.h
│   │   │   ├── ...
│   │   │   └── hk32f030m_wwdg.h
│   │   └── src
│   │       ├── hk32f030m_adc.c
│   │       ├── ...
│   │       └── hk32f030m_wwdg.c
│   └── LDScripts
```

## 4. Edit Makefile

Change the settings in Makefile, make sure ARM_TOOCHAIN and JLINKEXE points to the correct path

```makefile
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
LDSCRIPT		= Libraries/LDScripts/hk32f030mf4p6.ld
# Library build flags
LIB_FLAGS       = HK32F030MF4P6
```
## 5. Compiling And Flashing

```bash
# clean source code
make clean
# build
make
# or make with verbose output
V=1 make
# flash
make flash
```

# Try Other Examples

Replace the source files of *User* folder with the source files from other example folder.

# Reference

* Product Page: [https://www.hsxp-hk.com/product/98.html](https://www.hsxp-hk.com/product/98.html)
* Datasheet(Chinese) and User Manual(Chinese) Download: [https://www.hsxp-hk.com/companyfile/155/](https://www.hsxp-hk.com/companyfile/155/)
