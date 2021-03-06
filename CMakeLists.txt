# Copyright (c) 2013, Ford Motor Company
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following
# disclaimer in the documentation and/or other materials provided with the
# distribution.
#
# Neither the name of the Ford Motor Company nor the names of its contributors
# may be used to endorse or promote products derived from this software
# without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 2.8.0)

set (PROJECT smartDeviceLinkCore)
project (${PROJECT})

option(HMI2 "Use Qt HMI" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Please do not change compiler/linker flags if You do not know how particular
# flag is handled by CMake
set(CMAKE_INSTALL_PREFIX .)
set(ARCHIVE_OUTPUT_DIRECTORY ./bin)

option(EXTENDED_MEDIA_MODE "Turn on and off extended Madia Manager features relates to PulseAudio A2DP and GStreamer" OFF)
option(BUILD_SHARED_LIBS "Build all libraries as shared (if ON) or static (if OFF)" OFF)
option(BUILD_BT_SUPPORT "Bluetooth support" OFF)
option(BUILD_USB_SUPPORT "libusb support" OFF)
option(BUILD_AVAHI_SUPPORT "libavahi support" OFF)
option(BUILD_RWLOCK_SUPPORT "rwlocks support" OFF)
option(BUILD_BACKTRACE_SUPPORT "backtrace support" ON)


if (UNIX)
  add_definitions(-DOS_POSIX)

  set(CMAKE_CXX_FLAGS "-g3 -ggdb3 -std=gnu++0x -Werror=return-type -Wuninitialized")
  set(CMAKE_CXX_FLAGS_RELEASE "-s -O2") #It will be appended to CMAKE_CXX_FLAGS in release

  if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_definitions(-DOS_LINUX)
  elseif(CMAKE_SYSTEM_NAME STREQUAL "QNX")
    add_definitions(-DOS_QNX)
    SET(BUILD_BT_SUPPORT OFF)
    SET(BUILD_AVAHI_SUPPORT OFF)
    SET(BUILD_BACKTRACE_SUPPORT OFF)
    SET(EXTENDED_MEDIA_MODE OFF)
  elseif(CMAKE_SYSTEM_NAME STREQUAL Darwin)
    add_definitions(-DOS_MACOSX)
    #compile 32-bit 64-bit is having an issue
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch i386")
  endif()
endif(UNIX)

if (BUILD_USB_SUPPORT)
  add_definitions(-DUSB_SUPPORT)
endif()

if (BUILD_BT_SUPPORT)
  add_definitions(-DBLUETOOTH_SUPPORT)
endif()

if (BUILD_AVAHI_SUPPORT)
  # --- Check libavahi-common, libavahi-client availability
  find_package(Libavahi)

  add_definitions(-DAVAHI_SUPPORT)
endif()

if (BUILD_RWLOCK_SUPPORT)
  add_definitions(-DRWLOCK_SUPPORT)
endif()

if (BUILD_BACKTRACE_SUPPORT)
  add_definitions(-DBACKTRACE_SUPPORT)
endif()

# TODO(AK): check current OS here
add_definitions(-DOS_POSIX)

IF(CMAKE_C_FLAGS_DEBUG)
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG" )
    add_definitions(-DDEBUG)

    # Enable debug mode in the GNU C++ library, to catch incorrect/undefined behavior
    add_definitions(-D_GLIBCXX_DEBUG)
    add_definitions(-D_GLIBCXX_DEBUG_PEDANTIC)
ELSE (CMAKE_C_FLAGS_DEBUG)
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRELEASE" )
    add_definitions(-DRELEASE)
ENDIF(CMAKE_C_FLAGS_DEBUG)


if (EXTENDED_MEDIA_MODE)
add_definitions(-DEXTENDED_MEDIA)
# required to find 'glibconfig.h'
find_package(PkgConfig)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)
add_definitions(${GLIB2_CFLAGS})
endif()

# Find log4cxx
if(UNIX)
    find_package(PkgConfig)
    pkg_check_modules(LOG4CXX REQUIRED liblog4cxx)
    include_directories(${LOG4CXX_INCLUDE_DIRS})
    link_directories(${LOG4CXX_LIBRARY_DIRS})
elseif(WIN32)
#   Add finding log4cxx here
endif(UNIX)


# --- Interface generator

find_package(PythonInterp)
if(NOT PYTHONINTERP_FOUND)
    message(ERROR "Python interpreter is not found")
    message(ERROR "To install it type in the command line:")
    message(ERROR "sudo apt-get install python")
endif(NOT PYTHONINTERP_FOUND)

if (${HMI2})
    cmake_minimum_required(VERSION 2.8.11)
    if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
        set(qt_version "4.8.5")
    else ()
        set(qt_version "5.1.0")
    endif ()

    execute_process(
        COMMAND ${CMAKE_SOURCE_DIR}/FindQt.sh -v ${qt_version}
            OUTPUT_VARIABLE qt_bin_dir
    )
    message(STATUS "Binary directory Qt ${qt_version} is ${qt_bin_dir}")
    set(ENV{PATH} ${qt_bin_dir}:$ENV{PATH})

    if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
        find_package(Qt4 ${qt_version} REQUIRED QtCore QtGui QtDBus QtDeclarative)
    else ()
        find_package(Qt5Core REQUIRED)
        find_package(Qt5DBus REQUIRED)
        find_package(Qt5Qml REQUIRED)
        find_package(Qt5Quick REQUIRED)
        set(qmlplugindump_binary ${qt_bin_dir}/qmlplugindump)
    endif ()
endif()

set(INTEFRACE_GENERATOR "${PROJECT_SOURCE_DIR}/tools/InterfaceGenerator/Generator.py")
set(INTEFRACE_GENERATOR_CMD ${PYTHON_EXECUTABLE} -B ${INTEFRACE_GENERATOR})
file(GLOB_RECURSE INTERFACE_GENERATOR_DEPENDENCIES "${PROJECT_SOURCE_DIR}/tools/InterfaceGenerator/*.*")

macro (GenerateInterface arg_xml_name arg_namespace parser_type)
    string(REGEX MATCH "^[a-zA-Z_0-9]*[^.]" file_name ${arg_xml_name})     # TODO: make expression more robust

    set(hpp_file
      "${CMAKE_CURRENT_BINARY_DIR}/${file_name}.h"
      "${CMAKE_CURRENT_BINARY_DIR}/${file_name}_schema.h"
    )

    set(cpp_file "${CMAKE_CURRENT_BINARY_DIR}/${file_name}_schema.cc")
    set(full_xml_name "${CMAKE_CURRENT_SOURCE_DIR}/${arg_xml_name}")

    add_custom_command( OUTPUT ${hpp_file} ${cpp_file}
                        COMMAND ${INTEFRACE_GENERATOR_CMD} ${full_xml_name} ${arg_namespace} ${CMAKE_CURRENT_BINARY_DIR} "--parser-type" "${parser_type}"
                        DEPENDS ${INTERFACE_GENERATOR_DEPENDENCIES} ${full_xml_name}
                        COMMENT "Generating files:\n   ${hpp_file}\n   ${cpp_file}\nfrom:\n   ${arg_xml_name} ..."
                        VERBATIM
                       )

    include_directories (
        ../../../src/components/smart_objects/include
        ../../../src/components/formatters/include/
        ${CMAKE_SOURCE_DIR}/src/components/formatters/include/
        ${CMAKE_BINARY_DIR}
    )

    add_library("${file_name}" ${cpp_file})
endmacro(GenerateInterface)

# --- Useful macro
macro(create_test NAME SOURCES LIBS)
    add_executable("${NAME}" ${SOURCES})
    target_link_libraries("${NAME}" ${LIBS})
    add_test(${NAME} ${NAME})
endmacro(create_test)

# Building application

# --- Type HMI
if (${HMI2})
    set(QT_HMI ON)
    add_definitions(-DQT_HMI)
else (${HMI2})
    set(WEB_HMI ON)
    add_definitions(-DWEB_HMI)
ENDIF (${HMI2})

# --- ThirdPartyLibs
add_subdirectory(./src/thirdPartyLibs)

# --- Tools
add_subdirectory(./tools)

# --- Components
add_subdirectory(./src/components)

# --- Main application
add_subdirectory(./src/appMain)

# Building tests
option(BUILD_TESTS "Possibility to build and run tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    include(Dart)
    # Framework GoogleTest is also integrated together gmock
    # and must not be added separately
    add_subdirectory(./src/thirdPartyLibs/gmock-1.6.0)
    add_subdirectory(./test)
endif()

# Building documentation

# At first creating directory for generated documentation. Unfortunately doxygen
# cannot generate it byself
FIND_PACKAGE(Doxygen)
  IF(DOXYGEN_FOUND)
    option(DOXYGEN_ENABLE_DIAGRAMS "Enable graphical diagram generation" ON)

    if(DOXYGEN_ENABLE_DIAGRAMS)
      set(DOXYGEN_ENABLE_DIAGRAMS_PARAM "YES")
    else(DOXYGEN_ENABLE_DIAGRAMS)
      set(DOXYGEN_ENABLE_DIAGRAMS_PARAM "NO")
    endif()
    configure_file("${PROJECT_SOURCE_DIR}/Doxyfile" "${PROJECT_BINARY_DIR}/Doxyfile")
    file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/doc/doxygen")
    ADD_CUSTOM_TARGET(doxygen COMMAND ${DOXYGEN_EXECUTABLE} "${PROJECT_BINARY_DIR}/Doxyfile")
  ELSE(DOXYGEN_FOUND)
    MESSAGE(STATUS "Doxygen not found. Documentation will not be generated")
    MESSAGE(STATUS "To enable documentation generation please install doxygen and graphviz packages")
    MESSAGE(STATUS "sudo apt-get install doxygen graphviz")
    MESSAGE(STATUS "To enable processing of MscGen comments please install mscgen")
    MESSAGE(STATUS "sudo apt-get install mscgen")
ENDIF(DOXYGEN_FOUND)


