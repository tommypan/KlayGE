SET(LIB_NAME LibPNG)

SET(LIBPNG_SOURCE_FILES
	${FREEIMAGE_PROJECT_DIR}/Source/LibPNG/png.c
	${FREEIMAGE_PROJECT_DIR}/Source/LibPNG/pngerror.c
	${FREEIMAGE_PROJECT_DIR}/Source/LibPNG/pngget.c
	${FREEIMAGE_PROJECT_DIR}/Source/LibPNG/pngmem.c
	${FREEIMAGE_PROJECT_DIR}/Source/LibPNG/pngpread.c
	${FREEIMAGE_PROJECT_DIR}/Source/LibPNG/pngread.c
	${FREEIMAGE_PROJECT_DIR}/Source/LibPNG/pngrio.c
	${FREEIMAGE_PROJECT_DIR}/Source/LibPNG/pngrtran.c
	${FREEIMAGE_PROJECT_DIR}/Source/LibPNG/pngrutil.c
	${FREEIMAGE_PROJECT_DIR}/Source/LibPNG/pngset.c
	${FREEIMAGE_PROJECT_DIR}/Source/LibPNG/pngtrans.c
	${FREEIMAGE_PROJECT_DIR}/Source/LibPNG/pngwio.c
	${FREEIMAGE_PROJECT_DIR}/Source/LibPNG/pngwrite.c
	${FREEIMAGE_PROJECT_DIR}/Source/LibPNG/pngwtran.c
	${FREEIMAGE_PROJECT_DIR}/Source/LibPNG/pngwutil.c
)
SET(LIBPNG_HEADER_FILES
	${FREEIMAGE_PROJECT_DIR}/Source/LibPNG/png.h
	${FREEIMAGE_PROJECT_DIR}/Source/LibPNG/pngconf.h
	${FREEIMAGE_PROJECT_DIR}/Source/LibPNG/pngdebug.h
	${FREEIMAGE_PROJECT_DIR}/Source/LibPNG/pnginfo.h
	${FREEIMAGE_PROJECT_DIR}/Source/LibPNG/pnglibconf.h
	${FREEIMAGE_PROJECT_DIR}/Source/LibPNG/pngpriv.h
	${FREEIMAGE_PROJECT_DIR}/Source/LibPNG/pngstruct.h
)
SOURCE_GROUP("Source Files" FILES ${LIBPNG_SOURCE_FILES})
SOURCE_GROUP("Header Files" FILES ${LIBPNG_HEADER_FILES})

IF(KLAYGE_PLATFORM_WINDOWS)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
ENDIF()
INCLUDE_DIRECTORIES(${FREEIMAGE_PROJECT_DIR}/Source/zlib)
ADD_LIBRARY(${LIB_NAME} STATIC
	${LIBPNG_SOURCE_FILES} ${LIBPNG_HEADER_FILES}
)
ADD_DEPENDENCIES(${LIB_NAME} ZLib)

SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${FREEIMAGE_OUTPUT_DIR}
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${FREEIMAGE_OUTPUT_DIR}
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${FREEIMAGE_OUTPUT_DIR}
	ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${FREEIMAGE_OUTPUT_DIR}
	ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${FREEIMAGE_OUTPUT_DIR}
	PROJECT_LABEL ${LIB_NAME}
	DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
	OUTPUT_NAME ${LIB_NAME}${KLAYGE_OUTPUT_SUFFIX}
)

TARGET_LINK_LIBRARIES(${LIB_NAME})
