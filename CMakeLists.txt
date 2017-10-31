
cmake_minimum_required(VERSION 2.6)


project (BioBundle C CXX)


SET(CMAKE_CXX_FLAGS_COVERAGE
    "-g -O0 --coverage -fprofile-arcs -ftest-coverage -fno-inline -fno-inline-small-functions -fno-default-inline"
    CACHE STRING "Flags used by the C++ compiler during coverage builds."
    FORCE )
SET(CMAKE_C_FLAGS_COVERAGE
    "-g -O0 --coverage -fprofile-arcs -ftest-coverage -fno-inline -fno-inline-small-functions -fno-default-inline"
    CACHE STRING "Flags used by the C compiler during coverage builds."
    FORCE )
SET(CMAKE_EXE_LINKER_FLAGS_COVERAGE
    "--coverage"
    CACHE STRING "Flags used for linking binaries during coverage builds."
    FORCE )
SET(CMAKE_SHARED_LINKER_FLAGS_COVERAGE
    "--coverage"
    CACHE STRING "Flags used by the shared libraries linker during coverage builds."
    FORCE )
MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_COVERAGE
    CMAKE_C_FLAGS_COVERAGE
    CMAKE_EXE_LINKER_FLAGS_COVERAGE
    CMAKE_SHARED_LINKER_FLAGS_COVERAGE )


set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wunused -pedantic")

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release CACHE STRING
       "Choose the type of build, options are: Debug Coverage Release RelWithDebInfo MinSizeRel." FORCE )
endif()


set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED OFF)

if (WITH_UNIT_TEST)
	FIND_PACKAGE(Boost 1.46 COMPONENTS system filesystem iostreams program_options regex unit_test_framework REQUIRED)
else(WITH_UNIT_TEST)
	FIND_PACKAGE(Boost 1.46 COMPONENTS system filesystem iostreams program_options regex REQUIRED)
endif()
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
ADD_DEFINITIONS( "-DHAS_BOOST" )


FILE (GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/*.hpp)
list(REMOVE_ITEM HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/documentation.hpp)
FILE (GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)

add_library(BioBundleLib SHARED ${SOURCE_FILES})
#set_target_properties (BioBundleLib PROPERTIES VERSION 0.9)
target_link_libraries(BioBundleLib ${Boost_LIBRARIES})


INSTALL(TARGETS BioBundleLib
	DESTINATION "lib"
)


################################################
#                 tests                        #
################################################

if (WITH_UNIT_TEST)
	add_subdirectory (tests/unitTests)
	enable_testing ()
	add_test (NAME SequenceTest COMMAND seq_tests)
	add_test (NAME UtilityTest COMMAND utility_tests)
endif ()


################################################
#                    seqExtract                #
################################################

SET(seqExtract_src ./src/seqExtract.cpp ./src/lib/seq/Alphabets.cpp ./src/lib/seq/Sequence.cpp ./src/lib/seq/SequenceSet.cpp ./src/lib/external/Input.cpp ./src/lib/external/Output.cpp ./src/lib/utility/utility.cpp)
SET(seqExtract_exe seqExtract)
ADD_EXECUTABLE(${seqExtract_exe} ${seqExtract_src})
target_link_libraries(${seqExtract_exe}
	${Boost_LIBRARIES}
)


################################################
#                  isoformCleaner              #
################################################

SET(isoformCleaner_src ./src/isoformCleaner.cpp ./src/lib/seq/Sequence.cpp ./src/lib/seq/SequenceSet.cpp ./src/lib/external/Input.cpp ./src/lib/external/Output.cpp ./src/lib/utility/utility.cpp)
SET(isoformCleaner_exe isoformCleaner)
ADD_EXECUTABLE(${isoformCleaner_exe} ${isoformCleaner_src})
target_link_libraries(${isoformCleaner_exe}
	${Boost_LIBRARIES}
)

################################################
#                  seqSetMod              #
################################################

SET(seqSetMod_src ./src/seqSetMod.cpp ./src/lib/seq/Alphabets.cpp ./src/lib/seq/Sequence.cpp ./src/lib/seq/SequenceSet.cpp ./src/lib/external/Input.cpp ./src/lib/external/Output.cpp ./src/lib/utility/utility.cpp)
SET(seqSetMod_exe seqSetMod)
ADD_EXECUTABLE(${seqSetMod_exe} ${seqSetMod_src})
target_link_libraries(${seqSetMod_exe}
	${Boost_LIBRARIES}
)

################################################
#                    seqSetInfo                #
################################################

SET(seqSetInfo_src ./src/seqSetInfo.cpp ./src/lib/seq/Sequence.cpp ./src/lib/seq/SequenceSet.cpp ./src/lib/external/Input.cpp ./src/lib/external/Output.cpp ./src/lib/utility/utility.cpp)
SET(seqSetInfo_exe seqSetInfo)
ADD_EXECUTABLE(${seqSetInfo_exe} ${seqSetInfo_src})
target_link_libraries(${seqSetInfo_exe}
	${Boost_LIBRARIES}
)

################################################
#                    seqCheck                #
################################################

SET(seqCheck_src ./src/seqCheck.cpp ./src/lib/seq/Sequence.cpp ./src/lib/seq/SequenceSet.cpp ./src/lib/external/Input.cpp ./src/lib/external/Output.cpp ./src/lib/utility/utility.cpp)
SET(seqCheck_exe seqCheck)
ADD_EXECUTABLE(${seqCheck_exe} ${seqCheck_src})
target_link_libraries(${seqCheck_exe}
	${Boost_LIBRARIES}
)

################################################
#                    stopCleaner                #
################################################

SET(stopCleaner_src ./src/stopCleaner.cpp ./src/lib/seq/Sequence.cpp ./src/lib/seq/SequenceSet.cpp ./src/lib/external/Input.cpp ./src/lib/external/Output.cpp ./src/lib/utility/utility.cpp)
SET(stopCleaner_exe stopCleaner)
ADD_EXECUTABLE(${stopCleaner_exe} ${stopCleaner_src})
target_link_libraries(${stopCleaner_exe}
	${Boost_LIBRARIES}
)

################################################
#                    gffSort                   #
################################################

SET(gffSort_src ./src/gffSort.cpp ./src/lib/other/Feature.cpp
    ./src/lib/external/Input.cpp ./src/lib/external/Output.cpp ./src/lib/utility/utility.cpp)
SET(gffSort_exe gffSort)
ADD_EXECUTABLE(${gffSort_exe} ${gffSort_src})
target_link_libraries(${gffSort_exe}
	${Boost_LIBRARIES}
)


install(TARGETS ${seqExtract_exe} ${isoformCleaner_exe} ${seqSetInfo_exe} DESTINATION .)
