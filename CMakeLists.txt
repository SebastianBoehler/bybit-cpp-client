cmake_minimum_required(VERSION 3.16)
project(BybitCppClient VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)

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
    src/websocket_client.cpp
    src/websocket_topics.cpp
    src/http_error.cpp
    src/http_serialization.cpp
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

option(BYBIT_ENABLE_WEBSOCKET "Enable websocket client (requires ixwebsocket)" OFF)
if(BYBIT_ENABLE_WEBSOCKET)
    include(FetchContent)
    # Ensure ixwebsocket builds with TLS + OpenSSL backend.
    set(USE_TLS ON CACHE BOOL "Enable TLS in ixwebsocket" FORCE)
    set(USE_OPEN_SSL ON CACHE BOOL "Force OpenSSL backend in ixwebsocket" FORCE)
    set(IXWEBSOCKET_USE_TLS ON CACHE BOOL "Enable TLS for ixwebsocket" FORCE)
    FetchContent_Declare(
        ixwebsocket
        GIT_REPOSITORY https://github.com/machinezone/IXWebSocket.git
        GIT_TAG v11.3.3
        FIND_PACKAGE_ARGS
    )
    FetchContent_MakeAvailable(ixwebsocket)
    find_package(Threads REQUIRED)
    target_link_libraries(bybit_client PUBLIC ixwebsocket::ixwebsocket Threads::Threads)
    target_compile_definitions(bybit_client PUBLIC BYBIT_ENABLE_WEBSOCKET=1)
endif()

# Examples
add_executable(sui_leverage_cycle examples/sui_leverage_cycle.cpp)
target_link_libraries(sui_leverage_cycle PRIVATE bybit_client)
add_executable(instruments_info examples/instruments_info.cpp)
target_link_libraries(instruments_info PRIVATE bybit_client)
add_executable(basic_linear examples/basic_linear.cpp)
target_link_libraries(basic_linear PRIVATE bybit_client)
add_executable(market_kline examples/market_kline.cpp)
target_link_libraries(market_kline PRIVATE bybit_client)
add_executable(ws_market_data examples/ws_market_data.cpp)
target_link_libraries(ws_market_data PRIVATE bybit_client)
add_executable(ws_orderbook examples/ws_orderbook.cpp)
target_link_libraries(ws_orderbook PRIVATE bybit_client)
add_executable(ws_private_positions examples/ws_private_positions.cpp)
target_link_libraries(ws_private_positions PRIVATE bybit_client)
add_executable(ws_sbe_market examples/ws_sbe_market.cpp)
target_link_libraries(ws_sbe_market PRIVATE bybit_client)
add_executable(limit_order examples/limit_order.cpp)
target_link_libraries(limit_order PRIVATE bybit_client)
add_executable(position_info examples/position_info.cpp)
target_link_libraries(position_info PRIVATE bybit_client)
add_executable(wallet_balance examples/wallet_balance.cpp)
target_link_libraries(wallet_balance PRIVATE bybit_client)
add_executable(latency_benchmark examples/latency_benchmark.cpp)
target_link_libraries(latency_benchmark PRIVATE bybit_client)

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
        add_executable(bybit_public_test tests/test_public.cpp)
        target_link_libraries(bybit_public_test PRIVATE bybit_client)
        add_test(NAME bybit_public_test COMMAND bybit_public_test)
        add_executable(bybit_signing_test tests/test_signing.cpp)
        target_link_libraries(bybit_signing_test PRIVATE bybit_client)
        add_test(NAME bybit_signing_test COMMAND bybit_signing_test)
        add_executable(bybit_http_options_test tests/test_http_options.cpp)
        target_link_libraries(bybit_http_options_test PRIVATE bybit_client)
        add_test(NAME bybit_http_options_test COMMAND bybit_http_options_test)
        add_executable(bybit_market_api_test tests/test_market_api.cpp)
        target_link_libraries(bybit_market_api_test PRIVATE bybit_client)
        add_test(NAME bybit_market_api_test COMMAND bybit_market_api_test)
        add_executable(bybit_account_api_test tests/test_account_api.cpp)
        target_link_libraries(bybit_account_api_test PRIVATE bybit_client)
        add_test(NAME bybit_account_api_test COMMAND bybit_account_api_test)
        add_executable(bybit_advanced_earn_api_test tests/test_advanced_earn_api.cpp)
        target_link_libraries(bybit_advanced_earn_api_test PRIVATE bybit_client)
        add_test(NAME bybit_advanced_earn_api_test COMMAND bybit_advanced_earn_api_test)
        add_executable(bybit_affiliate_api_test tests/test_affiliate_api.cpp)
        target_link_libraries(bybit_affiliate_api_test PRIVATE bybit_client)
        add_test(NAME bybit_affiliate_api_test COMMAND bybit_affiliate_api_test)
        add_executable(bybit_broker_api_test tests/test_broker_api.cpp)
        target_link_libraries(bybit_broker_api_test PRIVATE bybit_client)
        add_test(NAME bybit_broker_api_test COMMAND bybit_broker_api_test)
        add_executable(bybit_card_api_test tests/test_card_api.cpp)
        target_link_libraries(bybit_card_api_test PRIVATE bybit_client)
        add_test(NAME bybit_card_api_test COMMAND bybit_card_api_test)
        add_executable(bybit_crypto_loan_api_test tests/test_crypto_loan_api.cpp)
        target_link_libraries(bybit_crypto_loan_api_test PRIVATE bybit_client)
        add_test(NAME bybit_crypto_loan_api_test COMMAND bybit_crypto_loan_api_test)
        add_executable(bybit_earn_api_test tests/test_earn_api.cpp)
        target_link_libraries(bybit_earn_api_test PRIVATE bybit_client)
        add_test(NAME bybit_earn_api_test COMMAND bybit_earn_api_test)
        add_executable(bybit_institutional_loan_api_test tests/test_institutional_loan_api.cpp)
        target_link_libraries(bybit_institutional_loan_api_test PRIVATE bybit_client)
        add_test(NAME bybit_institutional_loan_api_test COMMAND bybit_institutional_loan_api_test)
        add_executable(bybit_pre_upgrade_api_test tests/test_pre_upgrade_api.cpp)
        target_link_libraries(bybit_pre_upgrade_api_test PRIVATE bybit_client)
        add_test(NAME bybit_pre_upgrade_api_test COMMAND bybit_pre_upgrade_api_test)
        add_executable(bybit_rfq_api_test tests/test_rfq_api.cpp)
        target_link_libraries(bybit_rfq_api_test PRIVATE bybit_client)
        add_test(NAME bybit_rfq_api_test COMMAND bybit_rfq_api_test)
        add_executable(bybit_spot_margin_api_test tests/test_spot_margin_api.cpp)
        target_link_libraries(bybit_spot_margin_api_test PRIVATE bybit_client)
        add_test(NAME bybit_spot_margin_api_test COMMAND bybit_spot_margin_api_test)
        add_executable(bybit_spread_api_test tests/test_spread_api.cpp)
        target_link_libraries(bybit_spread_api_test PRIVATE bybit_client)
        add_test(NAME bybit_spread_api_test COMMAND bybit_spread_api_test)
        add_executable(bybit_strategy_api_test tests/test_strategy_api.cpp)
        target_link_libraries(bybit_strategy_api_test PRIVATE bybit_client)
        add_test(NAME bybit_strategy_api_test COMMAND bybit_strategy_api_test)
        add_executable(bybit_web3_api_test tests/test_web3_api.cpp)
        target_link_libraries(bybit_web3_api_test PRIVATE bybit_client)
        add_test(NAME bybit_web3_api_test COMMAND bybit_web3_api_test)
        add_executable(bybit_asset_api_test tests/test_asset_api.cpp)
        target_link_libraries(bybit_asset_api_test PRIVATE bybit_client)
        add_test(NAME bybit_asset_api_test COMMAND bybit_asset_api_test)
        add_executable(bybit_leverage_token_api_test tests/test_leverage_token_api.cpp)
        target_link_libraries(bybit_leverage_token_api_test PRIVATE bybit_client)
        add_test(NAME bybit_leverage_token_api_test COMMAND bybit_leverage_token_api_test)
        add_executable(bybit_websocket_api_test tests/test_websocket_api.cpp)
        target_link_libraries(bybit_websocket_api_test PRIVATE bybit_client)
        add_test(NAME bybit_websocket_api_test COMMAND bybit_websocket_api_test)
    endif()
endif()
