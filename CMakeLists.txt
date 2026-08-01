cmake_minimum_required(VERSION 3.16)
project(BybitCppClient VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)

file(GLOB_RECURSE BYBIT_STYLE_FILES CONFIGURE_DEPENDS
    include/*.hpp
    src/*.cpp
    tests/*.cpp
    examples/*.cpp
)
file(GLOB_RECURSE BYBIT_LINT_FILES CONFIGURE_DEPENDS
    src/*.cpp
    tests/*.cpp
    examples/*.cpp
)

find_program(CLANG_FORMAT_EXE NAMES clang-format HINTS
    /opt/homebrew/opt/llvm/bin
    /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin
)
if(CLANG_FORMAT_EXE)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE} -i ${BYBIT_STYLE_FILES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Formatting C++ sources with clang-format"
    )
    add_custom_target(format-check
        COMMAND ${CLANG_FORMAT_EXE} --dry-run --Werror ${BYBIT_STYLE_FILES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Checking C++ formatting with clang-format"
    )
else()
    add_custom_target(format
        COMMAND ${CMAKE_COMMAND} -E echo "clang-format not found; install it to run format"
    )
    add_custom_target(format-check
        COMMAND ${CMAKE_COMMAND} -E false
        COMMENT "clang-format not found; install it to run format-check"
    )
endif()

find_program(CLANG_TIDY_EXE NAMES clang-tidy HINTS /opt/homebrew/opt/llvm/bin)
if(CLANG_TIDY_EXE)
    set(BYBIT_CLANG_TIDY_ARGS --quiet -p ${CMAKE_BINARY_DIR})
    if(APPLE)
        execute_process(
            COMMAND xcrun --show-sdk-path
            OUTPUT_VARIABLE BYBIT_APPLE_SYSROOT
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )
        if(BYBIT_APPLE_SYSROOT)
            list(APPEND BYBIT_CLANG_TIDY_ARGS --extra-arg=-isysroot --extra-arg=${BYBIT_APPLE_SYSROOT})
        endif()
        list(APPEND BYBIT_CLANG_TIDY_ARGS --extra-arg=-stdlib=libc++)
    endif()
    add_custom_target(lint
        COMMAND ${CLANG_TIDY_EXE} ${BYBIT_CLANG_TIDY_ARGS} ${BYBIT_LINT_FILES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Linting C++ sources with clang-tidy"
    )
else()
    add_custom_target(lint
        COMMAND ${CMAKE_COMMAND} -E false
        COMMENT "clang-tidy not found; install it to run lint"
    )
endif()

add_library(bybit_client
    src/rest_client.cpp
    src/rest_client_advanced_earn.cpp
    src/rest_client_asset.cpp
    src/rest_client_broker.cpp
    src/rest_client_card.cpp
    src/rest_client_crypto_loan.cpp
    src/rest_client_earn.cpp
    src/rest_client_institutional_loan.cpp
    src/rest_client_leverage_token.cpp
    src/rest_client_market.cpp
    src/rest_client_pre_upgrade.cpp
    src/rest_client_rfq.cpp
    src/rest_client_spot_margin.cpp
    src/rest_client_spread.cpp
    src/rest_client_strategy.cpp
    src/rest_client_user.cpp
    src/rest_client_web3.cpp
    src/http_client.cpp
    src/public_rest_client.cpp
    src/public_rest_client_advanced_earn.cpp
    src/public_rest_client_earn.cpp
    src/public_rest_client_market.cpp
    src/public_rest_client_leverage_token.cpp
    src/private_rest_client.cpp
    src/private_rest_client_advanced_earn.cpp
    src/private_rest_client_account.cpp
    src/private_rest_client_asset.cpp
    src/private_rest_client_broker.cpp
    src/private_rest_client_card.cpp
    src/private_rest_client_crypto_loan.cpp
    src/private_rest_client_earn.cpp
    src/private_rest_client_institutional_loan.cpp
    src/private_rest_client_leverage_token.cpp
    src/private_rest_client_spot_margin.cpp
    src/private_rest_client_spread.cpp
    src/private_rest_client_strategy.cpp
    src/private_rest_client_user.cpp
    src/private_rest_client_history.cpp
    src/private_rest_client_position.cpp
    src/private_rest_client_rfq.cpp
    src/private_rest_client_trade.cpp
    src/private_rest_client_web3.cpp
    src/signing.cpp
    src/sbe_market_decoder.cpp
    src/sbe_order_entry_decoder.cpp
    src/sbe_order_entry_encoder.cpp
    src/websocket_auth.cpp
    src/websocket_client.cpp
    src/websocket_connection.cpp
    src/websocket_messages.cpp
    src/websocket_topics.cpp
    src/http_error.cpp
    src/http_serialization.cpp
)
target_compile_features(bybit_client PUBLIC cxx_std_17)

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

option(BYBIT_ENABLE_WEBSOCKET "Enable websocket client (requires ixwebsocket)" OFF)
if(BYBIT_ENABLE_WEBSOCKET)
    # Ensure ixwebsocket builds with TLS + OpenSSL backend.
    set(USE_TLS ON CACHE BOOL "Enable TLS in ixwebsocket" FORCE)
    set(USE_OPEN_SSL ON CACHE BOOL "Force OpenSSL backend in ixwebsocket" FORCE)
    set(IXWEBSOCKET_USE_TLS ON CACHE BOOL "Enable TLS for ixwebsocket" FORCE)
    # Upstream otherwise exports raw library paths for its static OpenSSL dependency.
    set(OPENSSL_LIBRARIES OpenSSL::SSL OpenSSL::Crypto)
    find_package(Threads REQUIRED)
    find_package(ZLIB REQUIRED)
    find_package(ixwebsocket CONFIG QUIET)
    if(NOT TARGET ixwebsocket::ixwebsocket)
        include(FetchContent)
        FetchContent_Declare(
            ixwebsocket
            GIT_REPOSITORY https://github.com/machinezone/IXWebSocket.git
            GIT_TAG v11.3.3
        )
        FetchContent_MakeAvailable(ixwebsocket)
        # Match the direct header directory exported by installed IX packages.
        target_include_directories(bybit_client PUBLIC
            "$<BUILD_INTERFACE:${ixwebsocket_SOURCE_DIR}/ixwebsocket>")
    endif()
    target_link_libraries(bybit_client PUBLIC ixwebsocket::ixwebsocket Threads::Threads)
    target_compile_definitions(bybit_client PUBLIC BYBIT_ENABLE_WEBSOCKET=1)
endif()

# Examples
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(BYBIT_BUILD_EXAMPLES_DEFAULT ON)
else()
    set(BYBIT_BUILD_EXAMPLES_DEFAULT OFF)
endif()
option(BYBIT_BUILD_EXAMPLES "Build examples" ${BYBIT_BUILD_EXAMPLES_DEFAULT})
include(cmake/BybitExamples.cmake)

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

include(cmake/BybitTests.cmake)
