## TC1796 HAL ##
set(halsrcs
	HalStartup.cpp
	RS232DeviceImpl.cpp
	AnalogDeviceImpl.cpp
	DigitalDeviceImpl.cpp
	DMAImpl.cpp
	I2CDeviceImpl.cpp
)

set(halhdrs
	RS232DeviceImpl.h
	AnalogDeviceImpl.h
	DigitalDeviceImpl.h
	DMAImpl.h
	I2CDeviceImpl.h
)

# include subdirectories
add_subdirectory(adc)
add_subdirectory(dsp)
add_subdirectory(gpta)
add_subdirectory(interfaces)
add_subdirectory(scu)
add_subdirectory(system)

add_library(hal ${halsrcs})

