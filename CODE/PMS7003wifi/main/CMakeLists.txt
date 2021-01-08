if("${TARGET}" STREQUAL "unit_tests")

else()
idf_component_register(SRCS "uart_events_example_main.cpp"
                    INCLUDE_DIRS ".")
	target_compile_options(${COMPONENT_LIB} PRIVATE -fpermissive -w)
endif()