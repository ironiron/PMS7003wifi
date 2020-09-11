
if("${TARGET}" STREQUAL "unit_tests")

add_library(PMS7003 PMS7003.cpp)
message("00000000000000000000000")
else()
message("11111111111111111111111")
idf_component_register(SRCS "PMS7003.cpp"
                    INCLUDE_DIRS ".")

endif()