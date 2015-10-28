Project(HelloWorld)

cmake_minimum_required(VERSION 2.8)

# use the eCos support coming with CMake
# without this line CMake doesn't know how to build eCos applications
# all functions/macros coming from this file start with "ECOS_"

INCLUDE(UseEcos)

#set (SCRIPT_DIR "$ENV{ECOS_REPOSITORY}/../tools/scripts")
set(CMAKE_MODULE_PATH $ENV{EZS_CMAKE_MODULE_DIR})
INCLUDE(ezs_ecos_tricore)

# add the ecos/install/include/ directory to the header search path
#ECOS_ADD_INCLUDE_DIRECTORIES()
#ECOS_USE_I386_ELF_TOOLS()

#set(COMMON_FLAGS "-nostartfiles -nostdlib -ffunction-sections -fdata-sections -fno-builtin -Wall -Wpointer-arith  -Wundef -fdata-sections")
#set(CMAKE_C_FLAGS "${COMMON_FLAGS}")
#set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -fno-rtti -fno-exceptions -fno-unwind-tables -Woverloaded-virtual" )

# use ADD_DEFINITIONS() to add the definitions (or switches) you like
add_definitions("-g")

set(EZS_ARCH tc1796)

# this is one of the most important lines
# here you list all source files of your application
# you can mix C and C++ files as you like
# you can also add the header files here, but you don't have to
# you can use relative or absolute paths as you like
include_directories(${PROJECT_SOURCE_DIR}/libEZS/include)
include_directories(${PROJECT_SOURCE_DIR}/libEZS/hal/include)
include_directories(${PROJECT_SOURCE_DIR}/libEZS/hal/tc1796)
include_directories(${PROJECT_SOURCE_DIR}/libEZS/hal/tc1796/adc)
include_directories(${PROJECT_SOURCE_DIR}/libEZS/hal/tc1796/dsp)
include_directories(${PROJECT_SOURCE_DIR}/libEZS/hal/tc1796/gpta)
include_directories(${PROJECT_SOURCE_DIR}/libEZS/hal/tc1796/interfaces)
include_directories(${PROJECT_SOURCE_DIR}/libEZS/hal/tc1796/scu)
include_directories(${PROJECT_SOURCE_DIR}/libEZS/hal/tc1796/system)
include_directories(${PROJECT_SOURCE_DIR}/libEZS/hal/drivers/)

set(LIBEZS_SOURCES
	libEZS/src/ezs_tracer.cpp
	libEZS/src/ezs_impure_ptr.c
	libEZS/hal/${EZS_ARCH}/DigitalDeviceImpl.cpp
	libEZS/hal/${EZS_ARCH}/HalStartup.cpp
	libEZS/hal/${EZS_ARCH}/interfaces/GPIO.cpp
	libEZS/hal/${EZS_ARCH}/system/SystemFrequency.cpp
	libEZS/hal/${EZS_ARCH}/system/SystemTimer.cpp
	libEZS/hal/${EZS_ARCH}/RS232DeviceImpl.cpp
	libEZS/hal/${EZS_ARCH}/I2CDeviceImpl.cpp
	libEZS/hal/${EZS_ARCH}/gpta/GPTA.cpp
	libEZS/hal/${EZS_ARCH}/gpta/DCM.cpp
	libEZS/hal/${EZS_ARCH}/gpta/FPC.cpp
	libEZS/hal/${EZS_ARCH}/libos.src/wdtcon.c
	libEZS/drivers/${EZS_ARCH}/ezs_gpio.c
	libEZS/drivers/${EZS_ARCH}/ezs_dac.cpp
	libEZS/drivers/${EZS_ARCH}/ezs_rs232.cpp
	libEZS/drivers/${EZS_ARCH}/ezs_counter.cpp
	libEZS/hal/drivers/DAC.cpp
	)
SET(SRC_LIST
	hello.c
	${LIBEZS_SOURCES}
	)

set(TGT "app")
# this is the most important line:
# tell CMake that we want to create an executable named "sampleapp"
# from the files listed in the variable SRC_LIST
ECOS_ADD_EXECUTABLE(${TGT} ${SRC_LIST})

#add_subdirectory(emu)
