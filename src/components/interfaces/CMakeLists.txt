include_directories (
  ../utils/include/
  ../formatters/include
)

GenerateInterface("v4_protocol_v1_2_no_extra.xml" "NsSmartDeviceLinkRPC::V1" "sdlrpcv1")
GenerateInterface("MOBILE_API.xml" "mobile_apis" "sdlrpcv2")

IF (${WEB_HMI})
  GenerateInterface("HMI_API.xml" "hmi_apis" "jsonrpc")
ENDIF (${WEB_HMI})

IF (${QT_HMI})
    set(hpp_file
      "${CMAKE_CURRENT_BINARY_DIR}/QT_HMI_API.h"
      "${CMAKE_CURRENT_BINARY_DIR}/QT_HMI_API_schema.h"
    )

    set(cpp_file "${CMAKE_CURRENT_BINARY_DIR}/QT_HMI_API_schema.cc")
    set(full_xml_name "${CMAKE_CURRENT_SOURCE_DIR}/QT_HMI_API.xml")

    add_custom_command( OUTPUT ${hpp_file} ${cpp_file}
                        COMMAND ${INTEFRACE_GENERATOR_CMD} ${full_xml_name} hmi_apis ${CMAKE_CURRENT_BINARY_DIR} "--parser-type" "jsonrpc"
                        DEPENDS ${INTERFACE_GENERATOR_DEPENDENCIES} ${full_xml_name}
			COMMENT "Generating files:\n   ${hpp_file}\n   ${cpp_file}\nfrom:\n   QT_HMI_API.xml ..."
                        VERBATIM
                       )

    include_directories (
        ../../../src/components/smart_objects/include
        ../../../src/components/formatters/include/
        ${CMAKE_SOURCE_DIR}/src/components/formatters/include/
        ${CMAKE_BINARY_DIR}
    )

    add_custom_command( 
          OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/HMI_API.h"
	  COMMAND sed "-e" "s/QT_HMI_API/HMI_API/g" "${CMAKE_CURRENT_BINARY_DIR}/QT_HMI_API.h" > "${CMAKE_CURRENT_BINARY_DIR}/HMI_API.h"
	  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/QT_HMI_API.h")
    add_custom_command( 
          OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/HMI_API_schema.h"
	  COMMAND sed "-e" "s/QT_HMI_API/HMI_API/g" "${CMAKE_CURRENT_BINARY_DIR}/QT_HMI_API_schema.h" > "${CMAKE_CURRENT_BINARY_DIR}/HMI_API_schema.h"
	  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/QT_HMI_API_schema.h")
    add_custom_command( 
          OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/HMI_API_schema.cc"
	  COMMAND sed "-e" "s/QT_HMI_API/HMI_API/g" "${CMAKE_CURRENT_BINARY_DIR}/QT_HMI_API_schema.cc" > "${CMAKE_CURRENT_BINARY_DIR}/HMI_API_schema.cc"
	  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/QT_HMI_API_schema.cc" "${CMAKE_CURRENT_BINARY_DIR}/HMI_API_schema.h" "${CMAKE_CURRENT_BINARY_DIR}/HMI_API.h")
  add_library(HMI_API "${CMAKE_CURRENT_BINARY_DIR}/HMI_API_schema.cc")

ENDIF (${QT_HMI})

if( BUILD_SHARED_LIBS )
  install (FILES ${CMAKE_CURRENT_BINARY_DIR}/libHMI_API.so DESTINATION lib)
  install (FILES ${CMAKE_CURRENT_BINARY_DIR}/libMOBILE_API.so DESTINATION lib)
  install (FILES ${CMAKE_CURRENT_BINARY_DIR}/libv4_protocol_v1_2_no_extra.so DESTINATION lib)
endif()

