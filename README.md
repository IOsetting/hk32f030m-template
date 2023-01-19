# HK32F030MF4P6 Project Template 

* HSXP HK32F030M template project for GNU Arm Embedded Toolchain
* Supported programmers: J-Link, DAPLink/PyOCD, ST-LINK/PyOCD
* Supported IDE: VSCode

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
├── Docs                        # Datasheet
├── Examples                    # Example code
├── flash.jlink                 # JLink download commander script
├── Libraries
│   ├── CMSIS
│   ├── Debug                   # SysTick delay and printf for debug
│   ├── FreeRTOS                # FreeRTOS library
│   ├── HK32F030M_Driver        # MCU peripheral driver
│   └── LDScripts
├── LICENSE
├── Makefile                    # Make config
├── Misc
│   └── vscode
├── README.md
├── rules.mk                    # Pre-defined rules include in Makefile 
└── User                        # User application code
```

# Requirements

* Board using HK32F030MF4P6
* Programmer
  * J-Link: J-Link OB programmer
  * PyOCD: DAPLink, J-Link or ST-LINK
* SEGGER J-Link Software and Documentation pack [https://www.segger.com/downloads/jlink/](https://www.segger.com/downloads/jlink/)
* PyOCD [https://pyocd.io/](https://pyocd.io/)
* GNU Arm Embedded Toolchain

# Building

## 1. Install GNU Arm Embedded Toolchain

Download the toolchain from [Arm GNU Toolchain Downloads](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) according to your pc architecture, extract the files

```bash
tar xvf gcc-arm-11.2-2022.02-x86_64-arm-none-eabi.tar.xz
cd /opt/gcc-arm/
sudo mv ~/Backup/linux/gcc-arm-11.2-2022.02-x86_64-arm-none-eabi/ .
sudo chown -R root:root gcc-arm-11.2-2022.02-x86_64-arm-none-eabi/
```
## 2. Option #1: Install SEGGER J-Link

Download and install JLink from [J-Link / J-Trace Downloads](https://www.segger.com/downloads/jlink/).

```bash
# installation command for .deb
sudo dpkg -i JLink_Linux_V770a_x86_64.deb
```
The default installation directory is */opt/SEGGER*

## 2. Option #2: Install PyOCD

Install from pip instead of apt repository because the version is 0.13.1+dfsg-1, which is too low to recognize J-Link probe

```bash
pip uninstall pyocd
```
This will install PyOCD into:
```
/home/[user]/.local/bin/pyocd
/home/[user]/.local/bin/pyocd-gdbserver
/home/[user]/.local/lib/python3.10/site-packages/pyocd-0.34.2.dist-info/*
/home/[user]/.local/lib/python3.10/site-packages/pyocd/*
```
.profile will take care of the PATH, run `source ~/.profile` to make pyocd command available

## 3. Clone This Repository

Clone this repository to local workspace
```bash
git clone https://github.com/IOsetting/hk32f030m-template.git
```

## 4. Edit Makefile

Change the settings in Makefile
* make sure ARM_TOOCHAIN points to the correct path
* If you use J-Link, FLASH_PROGRM can be jlink or pyocd
* If you use DAPLink or ST-LINK, set FLASH_PROGRM to pyocd

```makefile
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
FLASH_PROGRM	?= pyocd


##### Toolchains #######

# path to gcc arm
ARM_TOOCHAIN 	?= /opt/gcc-arm/arm-gnu-toolchain-12.2.mpacbti-bet1-x86_64-arm-none-eabi/bin

# path to JLinkExe
JLINKEXE		?= /opt/SEGGER/JLink/JLinkExe
# JLink device type: HK32F030MD4P6, HK32F030MF4U6, HK32F030MF4P6 or HK32F030MJ4M6
JLINK_DEVICE	?= HK32F030MF4P6
# path to PyOCD
PYOCD_EXE		?= pyocd
# PyOCD device type: hk32f030md4p6, hk32f030mf4p6, hk32f030mf4u6, hk32f030mj4m6
PYOCD_DEVICE	?= hk32f030mf4p6
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

More examples can be found in *Examples* folder, copy and replace the files under *User* folder to try different examples.

# Links

* HSXP HK32F030M Product Page: [https://www.hsxp-hk.com/product/98.html](https://www.hsxp-hk.com/product/98.html)
* Datasheet(English, Chinese) and User Manual(Chinese) Download: [https://www.hsxp-hk.com/companyfile/155/](https://www.hsxp-hk.com/companyfile/155/)
* HK32F030MF4P6_Fwlib-Template(Keil5) Download: [https://bbs.21ic.com/icview-3236132-1-1.html](https://bbs.21ic.com/icview-3236132-1-1.html)
