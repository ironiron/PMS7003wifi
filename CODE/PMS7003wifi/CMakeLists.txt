cmake_minimum_required(VERSION 3.14)


#set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -w")

if("${TARGET}" STREQUAL "unit_tests")
	project(PMS7003wifi)
	set(CMAKE_CXX_STANDARD 14)
	set(CMAKE_CXX_STANDARD_REQUIRED True)
	########################################################
	enable_testing()
	#check if already downloaded. Faster build time, and no internet required
	if ( NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/googletest-src")
		# Download and unpack googletest at configure time
		configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
		execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
		  RESULT_VARIABLE result
		  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
		if(result)
		  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
		endif()
		execute_process(COMMAND ${CMAKE_COMMAND} --build .
		  RESULT_VARIABLE result
		  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
		if(result)
		  message(FATAL_ERROR "Build step for googletest failed: ${result}")
		endif()
		
		# Prevent overriding the parent project's compiler/linker
		# settings on Windows
		set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	
	endif()
	######################################################   
	# Add googletest directly to our build. This defines
	# the gtest and gtest_main targets.
	add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
	                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
	                 )
	                 #EXCLUDE_FROM_ALL)
	######################################################                        
	add_subdirectory(components/PMS7003)
	
	# TESTS                          
	file(GLOB_RECURSE TEST_SRC *test.cpp)
	add_executable(PMS7003wifi_test ${TEST_SRC})
	target_link_libraries(PMS7003wifi_test PUBLIC PMS7003 gtest)
	#include(CTest)
	#add_test(PMS7003wifi_test PMS7003wifi_test)
	
else()
	include($ENV{IDF_PATH}/tools/cmake/project.cmake)
	project(PMS7003wifi)
	set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
endif()

