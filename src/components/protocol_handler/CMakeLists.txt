include_directories (
  ./include
  ../utils/include/
  ../transport_manager/include/
  ../connection_handler/include/
  ../config_profile/include/
)

set (SOURCES
    ./src/applink_protocol.cc
    ./src/rpc_type.cc
    ./src/raw_message.cc
    ./src/protocol_handler_impl.cc
    ./src/protocol_packet.cc
    ./src/protocol_payload.cc
    ./src/message_priority.cc
    ./src/service_type.cc
)

add_library("ProtocolHandler" ${SOURCES})

if( BUILD_SHARED_LIBS )
  install (FILES ${CMAKE_CURRENT_BINARY_DIR}/libProtocolHandler.so DESTINATION lib)
endif()
