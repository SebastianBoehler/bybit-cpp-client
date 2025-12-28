cmake_minimum_required(VERSION 3.16)
project(BybitCppClient VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)

add_library(bybit_client
    src/rest_client.cpp
    src/http_client.cpp
    src/public_rest_client.cpp
    src/private_rest_client.cpp
    src/signing.cpp
)

target_include_directories(bybit_client
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(bybit_client
    PUBLIC
        OpenSSL::SSL
        OpenSSL::Crypto
        CURL::libcurl
)

# Examples
add_executable(sui_leverage_cycle examples/sui_leverage_cycle.cpp)
target_link_libraries(sui_leverage_cycle PRIVATE bybit_client)
add_executable(instruments_info examples/instruments_info.cpp)
target_link_libraries(instruments_info PRIVATE bybit_client)
add_executable(basic_linear examples/basic_linear.cpp)
target_link_libraries(basic_linear PRIVATE bybit_client)

# Enable as FetchContent/git submodule consumer
include(GNUInstallDirs)
install(TARGETS bybit_client
    EXPORT BybitCppClientTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT BybitCppClientTargets
    FILE BybitCppClientTargets.cmake
    NAMESPACE Bybit::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/BybitCppClient
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/BybitCppClientConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/BybitCppClientConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/BybitCppClientConfig.cmake
    @ONLY)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/BybitCppClientConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/BybitCppClientConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/BybitCppClient
)

option(BYBIT_BUILD_TESTS "Build tests" ON)
if(BYBIT_BUILD_TESTS)
    include(CTest)
    if(BUILD_TESTING)
        add_executable(bybit_readonly_test tests/test_readonly.cpp)
        target_link_libraries(bybit_readonly_test PRIVATE bybit_client)
        add_test(NAME bybit_readonly_test COMMAND bybit_readonly_test)
        add_executable(bybit_public_test tests/test_public.cpp)
        target_link_libraries(bybit_public_test PRIVATE bybit_client)
        add_test(NAME bybit_public_test COMMAND bybit_public_test)
    endif()
endif()
