CMAKE_MINIMUM_REQUIRED(VERSION 3.6)

PROJECT(vcl.filesystem)

# External libraries
SUBDIRS(externals/ziplib)

SET(VCL_FILESYSTEM_READERS_INC
	src/vcl/filesystem/readers/archivefilereader.h
	src/vcl/filesystem/readers/volumefilereader.h
)
SET(VCL_FILESYSTEM_READERS_SRC
	src/vcl/filesystem/readers/archivefilereader.cpp
	src/vcl/filesystem/readers/volumefilereader.cpp
)

SET(VCL_FILESYSTEM_MP_INC
	src/vcl/filesystem/mountpoints/archivemountpoint.h
	src/vcl/filesystem/mountpoints/memorymountpoint.h
	src/vcl/filesystem/mountpoints/volumemountpoint.h
)
SET(VCL_FILESYSTEM_MP_SRC
	src/vcl/filesystem/mountpoints/archivemountpoint.cpp
	src/vcl/filesystem/mountpoints/memorymountpoint.cpp
	src/vcl/filesystem/mountpoints/volumemountpoint.cpp
)

SET(VCL_FILESYSTEM_UTIL_INC
	src/vcl/filesystem/util/archive.h
)
SET(VCL_FILESYSTEM_UTIL_SRC
	src/vcl/filesystem/util/archive.cpp
)

SET(VCL_FILESYSTEM_INC
	src/vcl/filesystem/filereader.h
	src/vcl/filesystem/filesystem.h
	src/vcl/filesystem/filewriter.h
	src/vcl/filesystem/mountpoint.h
)
SET(VCL_FILESYSTEM_SRC
	src/vcl/filesystem/filereader.cpp
	src/vcl/filesystem/filesystem.cpp
	src/vcl/filesystem/filewriter.cpp
	src/vcl/filesystem/mountpoint.cpp
)

SET(SOURCE
	${VCL_FILESYSTEM_MP_INC}
	${VCL_FILESYSTEM_MP_SRC}
	${VCL_FILESYSTEM_READERS_INC}
	${VCL_FILESYSTEM_READERS_SRC}
	${VCL_FILESYSTEM_UTIL_INC}
	${VCL_FILESYSTEM_UTIL_SRC}
	${VCL_FILESYSTEM_INC}
	${VCL_FILESYSTEM_SRC}
)

SOURCE_GROUP("mountpoints" FILES ${VCL_FILESYSTEM_MP_SRC} ${VCL_FILESYSTEM_MP_INC})
SOURCE_GROUP("readers" FILES ${VCL_FILESYSTEM_READERS_SRC} ${VCL_FILESYSTEM_READERS_INC})
SOURCE_GROUP("util" FILES ${VCL_FILESYSTEM_UTIL_SRC} ${VCL_FILESYSTEM_UTIL_INC})
SOURCE_GROUP("" FILES ${VCL_FILESYSTEM_SRC} ${VCL_FILESYSTEM_INC})

# Generate library
ADD_LIBRARY(vcl.filesystem STATIC ${SOURCE})
SET_TARGET_PROPERTIES(vcl.filesystem PROPERTIES FOLDER libs)
SET_TARGET_PROPERTIES(vcl.filesystem PROPERTIES DEBUG_POSTFIX _d)

TARGET_INCLUDE_DIRECTORIES(vcl.filesystem PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
TARGET_INCLUDE_DIRECTORIES(vcl.filesystem PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/externals/ziplib/Source)

TARGET_LINK_LIBRARIES(vcl.filesystem
	ziplib
)

# File System Unit Tests
OPTION(VCL_BUILD_TESTS "Build the unit tests" OFF)
IF (VCL_BUILD_TESTS)
	SUBDIRS(externals/gtest)
	SET(BUILD_GTEST ON CACHE BOOL "" FORCE)
	SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

	# Define the test files
	SET(VCL_FILESYSTEM_TEST_SRC
		test/basics.cpp
	)

	SOURCE_GROUP("" FILES ${VCL_FILESYSTEM_TEST_SRC})

	ADD_EXECUTABLE(vcl.filesystem.test
		${VCL_FILESYSTEM_TEST_SRC}
	)

	TARGET_LINK_LIBRARIES(vcl.filesystem.test
		vcl.filesystem
		gtest
		gtest_main
	)
	
ENDIF (VCL_BUILD_TESTS)
