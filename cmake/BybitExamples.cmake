if(BYBIT_BUILD_EXAMPLES)
    set(BYBIT_EXAMPLES
        sui_leverage_cycle
        instruments_info
        basic_linear
        market_kline
        ws_market_data
        ws_orderbook
        ws_private_positions
        ws_sbe_market
        limit_order
        position_info
        wallet_balance
        latency_benchmark
    )
    foreach(example IN LISTS BYBIT_EXAMPLES)
        add_executable(${example} examples/${example}.cpp)
        target_link_libraries(${example} PRIVATE bybit_client)
    endforeach()
endif()
