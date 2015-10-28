set(halsrcs
	${halsrcs}
	${CMAKE_CURRENT_SOURCE_DIR}/SystemFrequency.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SystemTimer.cpp
	PARENT_SCOPE
)

set(halhdrs
	${halhdrs}
	${CMAKE_CURRENT_SOURCE_DIR}/SystemFrequency.h
	${CMAKE_CURRENT_SOURCE_DIR}/SystemTimer.h
	${CMAKE_CURRENT_SOURCE_DIR}/DMA.h
	PARENT_SCOPE
)
