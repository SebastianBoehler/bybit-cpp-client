if(NOT CONFIG_DIR)
    message(FATAL_ERROR "CONFIG_DIR is required")
endif()

file(GLOB CONFIG_FILES "${CONFIG_DIR}/*config*.cmake")
foreach(CONFIG_FILE IN LISTS CONFIG_FILES)
    file(READ "${CONFIG_FILE}" CONTENTS)
    string(REGEX MATCH "INTERFACE_LINK_LIBRARIES \"[^\"]*\"" LINK_INTERFACE "${CONTENTS}")
    if(LINK_INTERFACE MATCHES "\"/" OR
       LINK_INTERFACE MATCHES ";/" OR
       LINK_INTERFACE MATCHES "LINK_ONLY:/" OR
       LINK_INTERFACE MATCHES "[A-Za-z]:[/\\\\]")
        message(FATAL_ERROR "Absolute link path in ${CONFIG_FILE}: ${LINK_INTERFACE}")
    endif()
endforeach()
