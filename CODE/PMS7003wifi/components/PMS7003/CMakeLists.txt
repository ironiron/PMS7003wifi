if("${TARGET}" STREQUAL "unit_tests")
	#include_directories("/fakes")

	add_subdirectory(fakes)
	include_directories(fakes)
	add_library(PMS7003 PMS7003.cpp)
	target_include_directories(PMS7003 INTERFACE  fakes)
	
else()
	idf_component_register(SRCS "PMS7003.cpp"
                    INCLUDE_DIRS ".")
endif()