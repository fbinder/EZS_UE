set(halsrcs
	${halsrcs}
	${CMAKE_CURRENT_SOURCE_DIR}/GPIO.cpp
	PARENT_SCOPE
)

set(halhdrs
	${halhdrs}
	${CMAKE_CURRENT_SOURCE_DIR}/GPIO.h
	${CMAKE_CURRENT_SOURCE_DIR}/RS232.h
	${CMAKE_CURRENT_SOURCE_DIR}/SSC.h
	PARENT_SCOPE
)

