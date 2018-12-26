
if((NOT MSVC_VERSION) OR (MSVC_VERSION GREATER_EQUAL 1900))
    add_subdirectory(./re2)
endif()
