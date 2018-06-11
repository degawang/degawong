 
CMAKE_MINIMUM_REQUIRED (VERSION 2.5)

AUX_SOURCE_DIRECTORY(. FILE_MLS_SRCS)

set(DIR_MLS_INCLUDE
   	 ./
	../../../mat
	../../../base
	../../../time
	../../../exception)

INCLUDE_DIRECTORIES(${DIR_MLS_INCLUDE})
foreach(SUB_DIR
	${DIR_MLS_INCLUDE}
	)	
	FILE(GLOB_RECURSE SUB_MLS_HEADERS  ${SUB_DIR}/*.h)
	set(FILE_MLS_HEADERS
		${SUB_MLS_HEADERS}
		${FILE_MLS_HEADERS})
endforeach()

SOURCE_GROUP("Head Files" FILES ${FILE_MLS_HEADERS}) 

ADD_LIBRARY(mls ${FILE_MLS_SRCS} ${FILE_MLS_HEADERS})

MESSAGE(STATUS "===============mls====================") 
