cmake_minimum_required(VERSION 3.16)
project(BybitPackageConsumer LANGUAGES CXX)

find_package(BybitCppClient CONFIG REQUIRED)

if(ixwebsocket_DIR)
    execute_process(
        COMMAND ${CMAKE_COMMAND}
            -DCONFIG_DIR=${ixwebsocket_DIR}
            -P ${CMAKE_CURRENT_LIST_DIR}/../assert_relocatable_config.cmake
        RESULT_VARIABLE RELOCATABLE_RESULT
    )
    if(NOT RELOCATABLE_RESULT EQUAL 0)
        message(FATAL_ERROR "ixwebsocket installed target contains an absolute link path")
    endif()
endif()

add_executable(bybit_package_consumer main.cpp)
target_link_libraries(bybit_package_consumer PRIVATE Bybit::bybit_client)
