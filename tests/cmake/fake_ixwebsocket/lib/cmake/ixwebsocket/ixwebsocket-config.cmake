if(NOT TARGET ixwebsocket::ixwebsocket)
    add_library(ixwebsocket::ixwebsocket INTERFACE IMPORTED)
endif()
set_property(TARGET ixwebsocket::ixwebsocket PROPERTY INTERFACE_LINK_LIBRARIES
    "Threads::Threads;ZLIB::ZLIB;OpenSSL::SSL;OpenSSL::Crypto")
