include_directories (
  ./include/
  ../TransportManager/include/
  ../ProtocolHandler/include/
  ../../thirdPartyLibs/logger/include
  ../../thirdPartyLibs/logger/log4cplus-1.1.0/include
  ${CMAKE_BINARY_DIR}/src/thirdPartyLibs/logger/log4cplus-1.1.0/include
)

set (SOURCES
    ./src/CConnectionHandler.cpp
    ./src/CConnection.cpp
    ./src/CDevice.cpp
)

add_library("connectionHandler" ${SOURCES})

if( BUILD_SHARED_LIBS )
  install (FILES ${CMAKE_CURRENT_BINARY_DIR}/libconnectionHandler.so DESTINATION lib)
endif()
