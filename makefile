#################################################
# MAKEFILE For STM32F4xxx Devices 				#
# (c) 20120630 Nemui Trinomius					#
# http://nemuisan.blog.bai.ne.jp				#
#################################################

# Environment Dependent!!! This Environment assure under WINDOWS !!
# Throw path into YOUR environments for each Operation Systems.

ifneq (,$(filter $(shell uname),Darwin Linux))

else
export PATH = %SYSTEMROOT%;$(TOOLDIR)/bin;$(OCDIR);$(DFUDIR);$(MAKEDIR)
endif

# Toolchain prefix (i.e arm-none-eabi -> arm-none-eabi-gcc.exe)
ifneq (,$(filter $(shell uname),Darwin Linux))
TCHAIN  = /usr/local/arm-cs-tools/bin/arm-none-eabi
else
TCHAIN  = "$(GNU_ARM_PATH)/bin/arm-none-eabi"
endif

# OpenOCD prefix
OCD		= openocd
# Select OpenOCD Transport
OCDMODE = SWD
#OCDMODE = JTAG

# Select SWD Debug Adapter
#DBG_ADAPTER  = STLINKV2
DBG_ADAPTER  = STLINKV2_1
#DBG_ADAPTER  = JTAGKEY2_SWD
#DBG_ADAPTER  = CMSIS-DAP
#DBG_ADAPTER  = VERSALOON

# OpenOCD For FT2232 Devices Special Setting
# If u use WinUSB as Device Driver,uncomment this!
MPSSE	= ftdi
#MPSSE	= 

# Development Tools based on GNU Compiler Collection
DEVTOOL = LAUNCHPAD
#DEVTOOL = BLEEDING_EDGE
#DEVTOOL = YAGARTO
#DEVTOOL = DEVKITARM
#DEVTOOL = SOURCERY

# Check BuildTools
ifeq ($(DEVTOOL),LAUNCHPAD)
 TOOLDIR = C:/Devz/ARM/Launchpad
 NANOLIB = --specs=nano.specs
  NANOLIB += -u _printf_float
  NANOLIB += -u _scanf_float
 REMOVAL = rm
else ifeq ($(DEVTOOL),BLEEDING_EDGE)
 TOOLDIR = C:/Devz/ARM/Bleeding-edge
 REMOVAL = rm
else ifeq ($(DEVTOOL),DEVKITARM)
 TOOLDIR = C:/Devz/ARM/devkitARM
 REMOVAL = rm
else ifeq ($(DEVTOOL),YAGARTO)
 TOOLDIR = C:/Devz/ARM/Yagarto
 REMOVAL = rm
else ifeq ($(DEVTOOL),SOURCERY)
 TOOLDIR = C:/Devz/ARM/Sourcery
 REMOVAL = rm
else
 $(error SET BUILD-TOOLS AT FIRST!!)
endif

# Set UNIX-Like Tools(CoreUtils) Directory
MAKEDIR = C:/Devz/Coreutils/bin

# Set Flasher and Debugger
ifneq (,$(filter $(shell uname),Darwin Linux))
	OCDIR	= /usr/local/openocd
else
	OCDIR	= C:\openocd
endif

#OCD_DBG = -c "debug_level 3"
ifeq ($(OCDMODE),SWD)
ifeq ($(DBG_ADAPTER),VERSALOON)
OCD_ARG = -s $(OCDIR)/tcl						\
		  -f interface/vsllink_swd.cfg			\
		  -f target/stm32f4x_flash.cfg
else ifeq ($(DBG_ADAPTER),CMSIS-DAP)
OCD_ARG = -s $(OCDIR)/tcl						\
		  -f interface/cmsis-dap.cfg			\
		  -f target/stm32f4x_flash.cfg
else ifeq ($(DBG_ADAPTER),STLINKV2_1)
OCD_ARG = -s $(OCDIR)/tcl						\
		  -f target/nucleo-f4_flash.cfg
else
OCD_ARG = -s $(OCDIR)/tcl						\
		  -f target/stm32f4discovery_flash.cfg
#		  -f target/stm32f429discovery_flash.cfg
endif
else
OCD_ARG = -s $(OCDIR)/tcl						\
		  -f interface/$(MPSSE)/jtagkey2.cfg 	\
          -f target/stm32f4x_flash.cfg
endif
OCD_CMD = $(OCD_DBG) $(OCD_ARG)


#Set DFUse Directory
DFUDIR	= C:/Devz/ARM/ST/DFUse/BIN
#Set Shell Definitions
WSHELL  = cmd
MSGECHO = echo
#Set GDB/Insight Directory
GDBDIR  = C:/Devz/ARM/insight/bin
INSIGHT = $(GDBDIR)/arm-none-eabi-insight
# Environment Dependent!!!


# OPTIMIZE Definition
#OPTIMIZE		= fast
OPTIMIZE		= 2

# FPU Definition
ifeq ($(DEVTOOL),LAUNCHPAD)
 # Launchpad has hard-float library in free!
 USING_FPU		= -mfloat-abi=hard  -mfpu=fpv4-sp-d16
 #USING_FPU		= -mfloat-abi=softfp  -mfpu=fpv4-sp-d16
 #USING_FPU		= -mfloat-abi=soft
else ifeq ($(DEVTOOL),BLEEDING_EDGE)
 # Bleeding-edge has hard-float library in free!
 USING_FPU		= -mfloat-abi=hard  -mfpu=fpv4-sp-d16
 #USING_FPU		= -mfloat-abi=softfp  -mfpu=fpv4-sp-d16
 #USING_FPU		= -mfloat-abi=soft
else
 USING_FPU		= -mfloat-abi=softfp  -mfpu=fpv4-sp-d16
 #USING_FPU		= -mfloat-abi=soft
endif

# GCC LTO Specific Option
ifneq ($(OPTIMIZE),0)
USE_LTO			= -flto-partition=none -fipa-sra
#USE_LTO			= -flto -fipa-sra
endif
# GCC Version Specific Options
ALIGNED_ACCESS	= -mno-unaligned-access
#FPREC_CONST	    = -fsingle-precision-constant
#ARMV7M_BOOST    = -mslow-flash-data


# Semihosting Definition
#USING_HOSTAGE   = USE_SEMIHOSTING
ifeq ($(USING_HOSTAGE),USE_SEMIHOSTING)
SEMIHOST_LIB = --specs=rdimon.specs -lrdimon
else
START_LIB    = -nostartfiles
endif


# Apprication Version
APP_VER = W.I.P


# Board and MPU Definitions
#EVAL_BOARD    	= USE_MAIN_V2
EVAL_BOARD    	= USE_MAIN_V3
#EVAL_BOARD    	= USE_STM32F4DISCOVERY
#EVAL_BOARD    	= USE_32F429IDISCOVERY

ifeq ($(EVAL_BOARD),USE_MAIN_V2)
 MPU_CLASS			= STM32F4XX
 MPU_MODEL			= STM32F40_41xxx
 SUBMODEL			= STM32F407VGT6
 HSE_CLOCK 			= 25000000
 USE_EXT_SRAM   	= 
 USE_TOUCH_SENCE 	=
 USE_FATFS_HAL		= USE_SDIO_AS_SDCARD
 STM32PLUS_Fn = STM32PLUS_F407
 
 DEBUG_PORT = "Usart1<Usart1InterruptFeature>, Usart1InterruptFeature"
 
else ifeq ($(EVAL_BOARD),USE_MAIN_V3)
 MPU_CLASS			= STM32F4XX
 MPU_MODEL			= STM32F40_41xxx
 SUBMODEL			= STM32F407VGT6
 HSE_CLOCK 			= 25000000
 USE_EXT_SRAM   	= 
 USE_TOUCH_SENCE 	=
 USE_FATFS_HAL		= USE_SDIO_AS_SDCARD
 STM32PLUS_Fn = STM32PLUS_F407
 
 #FT232
 #DEBUG_PORT = "Usart2_Remap1<Usart2InterruptFeature>, Usart2InterruptFeature"
 #XBee
 DEBUG_PORT = "Usart1_Remap1<Usart1InterruptFeature>, Usart1InterruptFeature"
 
 else ifeq ($(EVAL_BOARD),USE_STM32F4DISCOVERY)
 MPU_CLASS			= STM32F4XX
 MPU_MODEL			= STM32F40_41xxx
 SUBMODEL			= STM32F407VGT6
 HSE_CLOCK 			= 8000000
 USE_EXT_SRAM   	= 
 USE_TOUCH_SENCE 	=
 USE_FATFS_HAL		= USE_SPI_AS_MMC
 STM32PLUS_Fn = STM32PLUS_F407
 #DEBUG_PORT = "Usart2<Usart2InterruptFeature>, Usart2InterruptFeature"
 
else ifeq ($(EVAL_BOARD),USE_32F429IDISCOVERY)
 MPU_CLASS			= STM32F4XX
 MPU_MODEL			= STM32F429_439xx
 SUBMODEL			= STM32F429ZIT6
 HSE_CLOCK 			= 8000000
 USE_EXT_SDRAM   	= EXT_SDRAM_SUPPORT
 USE_TOUCH_SENCE 	= USE_STMPE811_I2C
 USE_FATFS_HAL		= USE_SPI_AS_MMC
 STM32PLUS_Fn = STM32PLUS_F429

else
 $(error TARGET BOARD IS NOT DEFINED!!)
endif
PERIF_DRIVER    = USE_STDPERIPH_DRIVER

# Use FreeRTOS?
OS_SUPPORT		= BARE_METAL
#OS_SUPPORT		= USE_FREERTOS

#USE_USB			= USE_USBHOST

STM32PLUS_DIR = ../stm32plus
ROSLIB_DIR = ./lib/ros_lib
RTOS_PATH	= ./lib/FreeRTOS

FWLIB  			= $(STM32PLUS_DIR)/fwlib/f4/stdperiph
ifeq ($(STM32PLUS_Fn),STM32PLUS_F407)
STM32PLUS_LIB_DIR = $(STM32PLUS_DIR)/build/small-f4-$(HSE_CLOCK)e-hard
STATIC_LIB   	= -lstm32plus-small-f4-$(HSE_CLOCK)e-hard
else 
	ifeq ($(STM32PLUS_Fn),STM32PLUS_F429)
		STM32PLUS_LIB_DIR = $(STM32PLUS_DIR)/build/small-f429-$(HSE_CLOCK)e-hard
		STATIC_LIB   	= -lstm32plus-small-f429-$(HSE_CLOCK)e-hard
	endif
endif



# Synthesis makefile Defines
DEFZ = $(MPU_CLASS) $(SUBMODEL) $(EVAL_BOARD) $(PERIF_DRIVER) $(VECTOR_START) \
	   $(USING_HOSTAGE) $(OS_SUPPORT) $(USE_EXT_SRAM) $(USE_EXT_SDRAM) $(USE_EXT_HEAP) $(UART_DEBUG)
		
DEFZ += $(STM32PLUS_Fn)
		
SYNTHESIS_DEFS	= $(addprefix -D,$(DEFZ)) 							\
				 -DARM_MATH_CM4										\
				 -DPACK_STRUCT_END=__attribute\(\(packed\)\) 		\
				 -DALIGN_STRUCT_END=__attribute\(\(aligned\(4\)\)\) \
				 -DMPU_SUBMODEL=\"$(SUBMODEL)\"						\
				 -DAPP_VERSION=\"$(APP_VER)\"						\
				 -DHSE_VALUE=$(HSE_CLOCK)UL

ifdef DEBUG_PORT
SYNTHESIS_DEFS	+= -DDEBUG_PORT=$(DEBUG_PORT)
endif

# TARGET definition
TARGET 		= main
TARGET_ELF  = $(TARGET).elf
TARGET_SREC = $(TARGET).s19
TARGET_HEX  = $(TARGET).hex
TARGET_BIN  = $(TARGET).bin
TARGET_LSS  = $(TARGET).lss
TARGET_DFU  = $(TARGET).dfu
TARGET_SYM  = $(TARGET).sym

# define CMSIS LIBRARY PATH
CMSISLIB 		= ./lib/CMSIS
CMSIS_DEVICE 	= $(CMSISLIB)/Device/ST/STM32F4xx

#CMSIS_CORE		= $(CMSISLIB)/Include
#CMSIS_DEVICE_INC = $(CMSISLIB)/Device/ST/STM32F4xx/Include

CMSIS_CORE		= $(STM32PLUS_DIR)/fwlib/f4/cmsis/Include
CMSIS_DEVICE_INC = $(STM32PLUS_DIR)/fwlib/f4/cmsis/Device/ST/STM32F4xx/Include

# include PATH
INCPATHS	 = 	./							\
				./inc						\
				$(FWLIB)/inc  				\
				$(CMSIS_DEVICE_INC)			\
				$(CMSIS_CORE)				\
				$(LIBINCDIRS)				\
				$(STM32PLUS_DIR)			\
				$(STM32PLUS_DIR)/include	\
				$(STM32PLUS_DIR)/include/stl\
				$(ROSLIB_DIR)
				
INCLUDES     = $(addprefix -I ,$(INCPATHS))

# Set library PATH
LIBPATHS     = $(FWLIB) $(USBLIB) $(STM32PLUS_LIB_DIR)
LIBRARY_DIRS = $(addprefix -L,$(LIBPATHS))
# if you use math-library, put "-lm" 
MATH_LIB	 =	-lm

# LinkerScript PATH
LINKER_PATH =  ./lib/linker
LINKER_DIRS = $(addprefix -L,$(LINKER_PATH)) 

# Object definition
OBJS 	 = $(CFILES:%.c=$(OUTDIR)/%.o)  $(CPPFILES:%.cpp=$(OUTDIR)/%.o) $(SFILES:%.s=$(OUTDIR)/%.o)

# C code PATH
SOURCE  = ./src
CFILES = $(wildcard $(SOURCE)/*.c) $(wildcard $(SOURCE)/utils/*.c)  $(wildcard $(SOURCE)/hardware/*.c)

# CPP code PATH
CPPFILES = $(wildcard $(SOURCE)/*.cpp) $(wildcard $(SOURCE)/utils/*.cpp)  $(wildcard $(SOURCE)/hardware/*.cpp)

CPPFILES += $(ROSLIB_DIR)/time.cpp $(ROSLIB_DIR)/duration.cpp

#/*----- FatFs library PATH -----*/	
FATFS = ./lib/ff
LIBINCDIRS += $(FATFS)
CFILES += \
 $(FATFS)/ff.c 							\
 $(FATFS)/ff_rtc_if.c 					\
 $(FATFS)/option/unicode.c				\
 $(FATFS)/option/syscall.c
ifeq ($(USE_FATFS_HAL),USE_SPI_AS_MMC)
CFILES += \
 $(FATFS)/mmc_stm32f4.c
else ifeq ($(USE_FATFS_HAL),USE_SDIO_AS_SDCARD)
CFILES += \
 $(FATFS)/sdio_stm32f4.c
else
 $(error MUST Select Hardware Abstraction Layer )
endif

#/*----- USB Host library PATH -----*/
ifneq ($(USE_USB),)

DEFZ += \
	USE_USB\
	$(USE_USB)

#Use TIM2 to count accurate time
#DEFZ += USE_ACCURATE_TIME

USB_OTG = ./lib/STM32_USB_OTG_Driver
LIBINCDIRS += \
	$(USB_OTG)/inc

CFILES += \
	$(USB_OTG)/src/usb_core.c		\
	$(USB_OTG)/src/usb_hcd_int.c	\
	$(USB_OTG)/src/usb_hcd.c
endif

ifeq ($(USE_USB),USE_USBHOST)
USB_HOST = ./lib/STM32_USB_HOST_Library
LIBINCDIRS += \
	$(USB_HOST)/Core/inc
CFILES += \
	$(wildcard $(USB_HOST)/Core/src/*.c)

INCPATHS += \
	./inc/hardware/usb
CFILES += \
	$(wildcard ./src/hardware/usb/*.c)
endif

#/*----- STARTUP code PATH -----*/
STARTUP_DIR = ./lib/startup
ifeq ($(OS_SUPPORT),USE_FREERTOS)
SFILES += \
	$(SOURCE)/startup_$(shell echo $(MPU_MODEL)|tr "[:upper:]" "[:lower:]")_rtos.s
	
CFILES += \
	$(RTOS_PATH)/croutine.c\
	$(RTOS_PATH)/event_groups.c\
	$(RTOS_PATH)/list.c\
	$(RTOS_PATH)/queue.c\
	$(RTOS_PATH)/tasks.c\
	$(RTOS_PATH)/timers.c\
	$(RTOS_PATH)/portable/GCC/ARM_CM4F/port.c	\
	$(RTOS_PATH)/portable/MemMang/heap_2.c

LIBINCDIRS += 								\
	$(RTOS_PATH)/include					\
	$(RTOS_PATH)/portable/GCC/ARM_CM4F
	
else
SFILES += \
	$(STARTUP_DIR)/startup_$(shell echo $(MPU_MODEL)|tr "[:upper:]" "[:lower:]").s
endif


#/*----- STM32 Debug library -----*/
ifeq ($(OPTIMIZE),0)
CFILES += \
 ./lib/IOView/stm32f4xx_io_view.c
else
endif

OUTDIR := Build
DEPS:= $(CFILES:%.c=$(OUTDIR)/%.d) $(CPPFILES:%.cpp=$(OUTDIR)/%.d)


# TOOLCHAIN SETTING
CC 			= $(TCHAIN)-gcc
CPP 		= $(TCHAIN)-g++
OBJCOPY 	= $(TCHAIN)-objcopy
OBJDUMP 	= $(TCHAIN)-objdump
SIZE 		= $(TCHAIN)-size
AR 			= $(TCHAIN)-ar
LD 			= $(TCHAIN)-g++
NM 			= $(TCHAIN)-nm
REMOVE		= $(REMOVAL) -f
REMOVEDIR 	= $(REMOVAL) -rf

# C and ASM FLAGS
C_CXXFLAGS  = -MMD -MP -mcpu=cortex-m4 -march=armv7e-m -mtune=cortex-m4
C_CXXFLAGS += -mthumb -mlittle-endian $(ALIGNED_ACCESS) $(ARMV7M_BOOST) $(FPREC_CONST)
C_CXXFLAGS += -mapcs-frame -mno-sched-prolog $(USING_FPU)
C_CXXFLAGS += -gdwarf-2 -O$(OPTIMIZE) $(USE_LTO) $(NANOLIB) $(SEMIHOST_LIB)
C_CXXFLAGS += -fno-strict-aliasing -fsigned-char
C_CXXFLAGS += -ffunction-sections -fdata-sections
C_CXXFLAGS += -fno-schedule-insns2
C_CXXFLAGS += --param max-inline-insns-single=1000
C_CXXFLAGS += -fno-common
C_CXXFLAGS += -Wall -Wno-array-bounds -Wno-unused-but-set-variable
#C_CXXFLAGS += -Wdouble-promotion
#C_CXXFLAGS += -Wredundant-decls -Wreturn-type -Wshadow -Wunused
C_CXXFLAGS += -Wa,-adhlns=$(OUTDIR)/$(subst $(suffix $<),.lst,$<) 
C_CXXFLAGS += $(SYNTHESIS_DEFS)
C_CXXFLAGS += -g3

CFLAGS = -lstdc++ -std=gnu99

CXXFLAGS = -std=gnu++11 -fpermissive -fsigned-char -fno-rtti -fexceptions -fmessage-length=0 

# Linker FLAGS
LDFLAGS  = -mcpu=cortex-m4 -march=armv7e-m -mthumb
LDFLAGS += -u g_pfnVectors -Wl,-static -Wl,--gc-sections,-cref $(START_LIB)
LDFLAGS += -Wl,-Map=$(TARGET).map
LDFLAGS += $(LIBRARY_DIRS) $(LINKER_DIRS) $(MATH_LIB)
LDFLAGS +=-T$(LINKER_PATH)/$(SUBMODEL).ld

# Object Copy and dfu generation FLAGS
OBJCPFLAGS = -O
OBJDUMPFLAGS = -h -S -C
DFU	  = hex2dfu
DFLAGS = -w


# Build Object
all: gccversion build buildinform sizeafter
build: $(TARGET_ELF) $(TARGET_LSS) $(TARGET_SYM) $(TARGET_HEX) $(TARGET_SREC) $(TARGET_BIN)

.SUFFIXES: .o .c .cpp .s   

$(TARGET_LSS): $(TARGET_ELF)
	@$(MSGECHO)
	@$(MSGECHO) Disassemble: $@
	$(OBJDUMP) $(OBJDUMPFLAGS) $< > $@ 
$(TARGET_SYM): $(TARGET_ELF)
	@$(MSGECHO)
	@$(MSGECHO) Symbol: $@
	$(NM) -n $< > $@
$(TARGET).hex: $(TARGET).elf
	@$(MSGECHO)
	@$(MSGECHO) Objcopy: $@
	$(OBJCOPY) $(OBJCPFLAGS) ihex $^ $@    
$(TARGET).s19: $(TARGET).elf
	@$(MSGECHO)
	@$(MSGECHO) Objcopy: $@
	$(OBJCOPY) $(OBJCPFLAGS) srec $^ $@ 
$(TARGET).bin: $(TARGET).elf
	@$(MSGECHO)
	@$(MSGECHO) Objcopy: $@
	$(OBJCOPY) $(OBJCPFLAGS) binary $< $@ 
$(TARGET).dfu: $(TARGET).hex
	@$(MSGECHO)
	@$(MSGECHO) Make STM32 dfu: $@
	$(DFU) $(DFLAGS) $< $@
	@$(MSGECHO)
$(TARGET).elf: $(OBJS) 
	@$(MSGECHO) Link: $@
	$(LD) $(C_CXXFLAGS) $(LDFLAGS) $^ -o $@ $(STATIC_LIB)
	@$(MSGECHO)
$(OUTDIR)/%.o:%.c
ifneq (,$(filter $(shell uname),Darwin Linux))
	@if [ ! -e `/usr/bin/dirname $@` ]; then /bin/mkdir -p `/usr/bin/dirname $@`; fi
else
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
endif
	@$(MSGECHO) Compile: $<
	$(CC) -c $(CFLAGS) $(C_CXXFLAGS) $(INCLUDES) $< -o $@
	@$(MSGECHO)
$(OUTDIR)/%.o:%.cpp
ifneq (,$(filter $(shell uname),Darwin Linux))
	@if [ ! -e `/usr/bin/dirname $@` ]; then /bin/mkdir -p `/usr/bin/dirname $@`; fi
else
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
endif
	@$(MSGECHO) Compile: $<
	$(CPP) -c $(C_CXXFLAGS) $(CXXFLAGS) $(INCLUDES) $< -o $@
	@$(MSGECHO)
$(OUTDIR)/%.o:%.s
ifneq (,$(filter $(shell uname),Darwin Linux))
	@if [ ! -e `/usr/bin/dirname $@` ]; then /bin/mkdir -p `/usr/bin/dirname $@`; fi
else
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
endif
	@$(MSGECHO) Assemble: $<
	$(CC) -c $(C_CXXFLAGS) $(INCLUDES) $< -o $@
	@$(MSGECHO)

# Object Size Informations
sizeafter: $(TARGET).elf $(TARGET).hex buildinform
	@$(MSGECHO) 
	@$(MSGECHO) Built Object Informations:
	@$(MSGECHO) === Total Binary Size ===
	@$(SIZE) $(TARGET).hex
	@$(MSGECHO) === Verbose ELF Size ===
	@$(SIZE) $(TARGET).elf
	@$(SIZE) -A -x $(TARGET).elf

# Display compiler version information.
gccversion : 
	@$(CC) --version
	@$(MSGECHO) 

buildinform :
	@$(MSGECHO) 
	@$(MSGECHO) 
	@$(MSGECHO) Built Informations:
	@$(MSGECHO) TCHAIN_PATH = $(TCHAIN)
	@$(MSGECHO) EVAL_BOARD = $(EVAL_BOARD)
	@$(MSGECHO) MPU_CLASS = $(MPU_CLASS)
	@$(MSGECHO) MPU_MODEL = $(MPU_MODEL)
	@$(MSGECHO) SUBMODEL = $(SUBMODEL)
	@$(MSGECHO) HSE_CLOCK = $(HSE_CLOCK)
	@$(MSGECHO) USING_FPU = $(USING_FPU)
	@$(MSGECHO) USING_SYSTEM = $(OS_SUPPORT)
	@$(MSGECHO) DEBUG_PORT = $(DEBUG_PORT)

# Flash and Debug Program
debug :
	$(WSHELL) /c start /B $(INSIGHT) $(TARGET).elf
	$(OCD) $(OCD_CMD) -c "reset halt" -c"arm semihosting enable"
program :
ifneq (,$(filter $(shell uname),Darwin Linux))
#	@cp $(TARGET).elf $(OCDIR)
#	@cd $(OCDIR) && ./$(OCD) $(OCD_CMD) -c "mt_flash $(TARGET).elf"
	@$(OCDIR)/bin/$(OCD) $(OCD_CMD) -c "mt_flash_bin $(TARGET).bin 0x08000000"
else
	$(OCD) $(OCD_CMD) -c "mt_flash $(TARGET).elf"
#	$(OCD) $(OCD_CMD) -c "eraser"
#	$(OCD) $(OCD_CMD) -c "mt_flash_bin $(TARGET).bin 0x08000000"
endif

ifndef STM32_ST_LINK_Utility_PATH
  STM32_ST_LINK_Utility_PATH = C:/ST-LINK_Utility
endif

flash :
ifneq (,$(filter $(shell uname),Darwin Linux))
	@echo "Writing $(TARGET).bin"
	@st-flash --reset write $(TARGET).bin 0x08000000
else
	@"$(STLINK_PATH)/ST-LINK_CLI.exe" -c SWD UR -P $(TARGET).hex 0x08000000
	@"$(STLINK_PATH)/ST-LINK_CLI.exe" -c SWD UR -Rst
endif

# Drop files into dust-shoot
.PHONY clean:
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).bin
	$(REMOVE) $(TARGET).obj
	$(REMOVE) $(TARGET).map
	$(REMOVE) $(TARGET).s19
	$(REMOVE) $(TARGET).a90
	$(REMOVE) $(TARGET).sym
	$(REMOVE) $(TARGET).lnk
	$(REMOVE) $(TARGET).lss
	$(REMOVE) $(TARGET).dfu
	$(REMOVE) $(wildcard *.stackdump)
	$(REMOVEDIR) .dep
	$(REMOVEDIR) $(OUTDIR)
	@$(MSGECHO)

-include $(DEPS)

# Listing of phony targets.
.PHONY : all begin finish end sizebefore sizeafter gccversion \
build elf hex bin lss sym clean clean_list program
