/**
 * \file CDevice.hpp
 * \brief CDevice class.
 * Stores device information
 *
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DEVICE_H 
#define DEVICE_H 

#include "Logger.hpp"
#include <string>
#include <map>

/**
 * \namespace NsConnectionHandler
 * \brief SmartDeviceLink ConnectionHandler namespace.
 */ 
namespace NsConnectionHandler
{

    /**
     * \brief Type for DeviceHandle
     */
    typedef int tDeviceHandle;

    /**
    * \class CDevice
    * \brief Connection class
    */
    class CDevice
    {
    public:
        /**
         * \brief Class constructor
         */
        CDevice(tDeviceHandle aDeviceHandle, std::string aUserFriendlyName);

        /**
         * \brief Destructor
         */
        ~CDevice();

        /**
         * \brief Returns device handle
         * \return DeviceHandle
         */
        tDeviceHandle getDeviceHandle() const;

        /**
         * \brief Returns user frendly device name
         * \return UserFriendlyName
         */
        std::string getUserFriendlyName() const;

    private:
        /**
         * \brief Uniq device handle.
         */
        tDeviceHandle mDeviceHandle;

        /**
         * \brief User-friendly device name.
         */
        std::string mUserFriendlyName;

        /**
         * \brief For logging.
         */
        static log4cplus::Logger mLogger;
    };

    /**
     * \brief Type for Devices map
     */
    typedef std::map<int, CDevice> tDeviceList;

    /**
     * \brief Type for Devices map iterator
     * Key is DeviceHandle which is uniq
     */
    typedef std::map<int, CDevice>::iterator tDeviceListIterator;

}/* namespace NsConnectionHandler */

#endif /* DEVICE_H */
