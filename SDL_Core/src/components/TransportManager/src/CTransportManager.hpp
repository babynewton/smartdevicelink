/**
 * \file CTransportManager.hpp
 * \brief Class CTransportManager header.
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

#ifndef __TRANSPORTMANAGER_CTRANSPORTMANAGER_HPP__
#define __TRANSPORTMANAGER_CTRANSPORTMANAGER_HPP__

#include "TransportManager/ITransportManager.hpp"
#include "IDeviceAdapterListener.hpp"
#include "IHandleGenerator.hpp"
#include "Logger.hpp"

#include <pthread.h>
#include <vector>

namespace NsSmartDeviceLink
{
    namespace NsTransportManager
    {
        class IDeviceAdapter;

        // Some constants

        const uint8_t PROTOCOL_HEADER_V1_SIZE = 8;
        const uint8_t PROTOCOL_HEADER_V2_SIZE = 12;

        const uint8_t PROTOCOL_VERSION_1 = 1;
        const uint8_t PROTOCOL_VERSION_2 = 2;

        // Class definition

        /**
         * @brief Transport manager implementation.
         *
         * Potential future optimizations:
         *
         * @todo Potential future optimization: Add shutdown flag checking inside client interface function calls
         * @todo Potential future optimization: Implement more thread-safe processing of shutdown flag
         * @todo Potential future optimization: Move ITransportManagerDeviceListener::onApplicationConnected and
         *       ITransportManagerDeviceListener::onApplicationDisconnected callbacks calling to corresponded connection thread
         *
         * @todo Potential future optimization: Currently all frames processed in one thread. In the future processing of them
         *       can be moved to the thread, which sent callbacks for corresponded connection.
         *
         * @todo Potential future optimization: Currently it is not possible to validate incoming frame data. Some kind of data
         *       validation can be implemented and if incoming data is invalid one of the possible solutions is to close
         *       corresponded connection
         **/
        class CTransportManager: public ITransportManager,
                                 public IDeviceAdapterListener,
                                 public IHandleGenerator
        {
        public:
            /**
             * @brief Constructor.
             **/
            CTransportManager(void);

            /**
             * @brief Destructor.
             **/
            virtual ~CTransportManager(void);

            /**
             * @brief Run transport manager.
             *
             * Must be called from startup after all references to
             * external components are initialized to start trasport manager.
             **/
            virtual void run(void);

            /**
             * @brief Start scanning for new devices.
             *
             * List of new devices will be supplied in onDeviceListUpdated callback.
             **/
            virtual void scanForNewDevices(void);

            /**
             * @brief Connect to all applications discovered on device.
             *
             * @param DeviceHandle Handle of device to connect to.
             **/
            virtual void connectDevice(const tDeviceHandle DeviceHandle);

            /**
             * @brief Disconnect from all applications connected on device.
             *
             * @param DeviceHandle Handle of device to disconnect from.
             **/
            virtual void disconnectDevice(const tDeviceHandle DeviceHandle);

            /**
             * @brief Add listener to the data-related events
             *
             * @param Listener Pointer to listener.
             **/
            virtual void addDataListener(ITransportManagerDataListener * Listener);

            /**
             * @brief Remove listener to the data-related events.
             *
             * @param Listener Pointer to listener.
             **/
            virtual void removeDataListener(ITransportManagerDataListener * Listener);

            /**
             * @brief Add listener to the device-related events.
             *
             * @param Listener Pointer to listener.
             **/
            virtual void addDeviceListener(ITransportManagerDeviceListener * Listener);

            /**
             * @brief Remove listenerto the device-related events.
             *
             * @param Listener Pointer to listener.
             **/
            virtual void removeDeviceListener(ITransportManagerDeviceListener * Listener);

            /**
             * @brief Send frame.
             *
             * @param ConnectionHandle Connection handle.
             * @param Data Frame payload data.
             * @param DataSize Size of data in bytes.
             * @param UserData Any user data. Will be returned as is in ITransportManagerDataListener::onFrameSendCompleted
             **/
            virtual void sendFrame(tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize, const int UserData);

            /**
             * @brief Generate new device handle.
             *
             * Internal method used for generation of unique device handle
             * for device adapters.
             *
             * @return New device handle.
             **/
            virtual tDeviceHandle generateNewDeviceHandle(void);

            /**
             * @brief Generate new connection handle.
             *
             * Internal method used for generation of unique connection handle
             * for device adapters.
             *
             * @return New connection handle.
             **/
            virtual tConnectionHandle generateNewConnectionHandle(void);

            // Handling Device Adapters callbacks

            /**
             * @brief Available devices list updated callback.
             *
             * @param DeviceAdapter Calling device adapter.
             * @param DeviceList New list of available devices.
             **/
            virtual void onDeviceListUpdated(IDeviceAdapter * DeviceAdapter, const tInternalDeviceList & DeviceList);

            /**
             * @brief Application connected callback.
             *
             * @param ConnectedDevice DeviceInfo of device with connected application.
             * @param Connection Connection handle.
             **/
            virtual void onApplicationConnected(IDeviceAdapter * DeviceAdapter, const SDeviceInfo & ConnectedDevice, const tConnectionHandle ConnectionHandle);

            /**
             * @brief Application disconnected callback.
             *
             * @param ConnectedDevice DeviceInfo of device with connected application.
             * @param Connection Connection handle.
             **/
            virtual void onApplicationDisconnected(IDeviceAdapter * DeviceAdapter, const SDeviceInfo & DisconnectedDevice, const tConnectionHandle ConnectionHandle);

            /**
             * @brief Frame received callback.
             *
             * @param DeviceAdapter Calling device adapter.
             * @param ConnectionHandle Connection handle.
             * @param Data Received frame data.
             * @param DataSize Size of received data in bytes.
             *
             * @note  %Data pointed by parameter Data is valid only until
             *        this callback returns control to the caller. Callback
             *        handler is responsible to copy this data to its own
             *        location if it needs the data after return.
             **/
            virtual void onFrameReceived(IDeviceAdapter * DeviceAdapter, tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize);

            /**
             * @brief Frame send completed callback.
             *
             * @param DeviceAdapter Calling device adapter.
             * @param ConnectionHandle Connection handle.
             * @param UserData User data that was previously passed to ITransportManager::sendFrame.
             * @param SendStatus Result status.
             **/
            virtual void onFrameSendCompleted(IDeviceAdapter * DeviceAdapter, tConnectionHandle ConnectionHandle, int UserData, ESendStatus SendStatus);

        protected:

            /**
             * @brief Enumeration that represents type of the device listener callback
             *        that must be called
             **/
            enum EDeviceListenerCallbackType
            {
                DeviceListenerCallbackType_DeviceListUpdated = 0,
                DeviceListenerCallbackType_ApplicationConnected = 1,
                DeviceListenerCallbackType_ApplicationDisconnected = 2
            };

            /**
             * @brief Enumeration that represents type of the data listener callback
             *        that must be called
             **/
            enum EDataListenerCallbackType
            {
                DataListenerCallbackType_FrameReceived = 0,
                DataListenerCallbackType_FrameSendCompleted = 1
            };

            /**
             * @brief Device listener callback information
             *
             * Used to store data for calling device listeners
             **/
            struct SDeviceListenerCallback
            {
                /**
                 * @brief Constructor for creating DeviceListUpdated callback.
                 *
                 * @param CallbackType Type of the callback
                 * @param DeviceList Device list
                 **/
                SDeviceListenerCallback(EDeviceListenerCallbackType CallbackType, const tDeviceList & DeviceList);

                /**
                 * @brief Constructor for creating ApplicationConnected and ApplicationDisconnected callbacks
                 *
                 * @param CallbackType Type of the callback
                 * @param DeviceInfo Device information
                 * @param ConnectionHandle Connection handle
                 **/
                SDeviceListenerCallback(EDeviceListenerCallbackType CallbackType, const SDeviceInfo & DeviceInfo, const tConnectionHandle & ConnectionHandle);

                /**
                 * @brief Copy constructor
                 *
                 * @param other Reference to object to be copied
                 **/
                SDeviceListenerCallback(const SDeviceListenerCallback& other);

                /**
                 * @brief Comparison operator.
                 *
                 * @param i_other Reference to the object to be compared with
                 * @return bool
                 **/
                bool operator==( const SDeviceListenerCallback& i_other ) const;

                /**
                 * @brief Destructor.
                 *
                 **/
                ~SDeviceListenerCallback(void);

                /**
                 * @brief Type of the callback that must be called
                 **/
                EDeviceListenerCallbackType mCallbackType;

                /**
                 * @brief Device list (will be sent in the DeviceListUpdated callback)
                 **/
                tDeviceList mDeviceList;

                /**
                 * @brief Device information (will be sent in the ApplicationConnected and ApplicationDisconnected callbacks)
                 **/
                SDeviceInfo mDeviceInfo;

                /**
                 * @brief Connection handle (will be sent in the ApplicationConnected and ApplicationDisconnected callbacks)
                 **/
                tConnectionHandle mConnectionHandle;
            };

            /**
             * @brief Data listener callback information
             *
             * Used to store data for calling data listeners
             **/
            struct SDataListenerCallback
            {
                /**
                 * @brief Constructor for creating FrameReceived callback.
                 *
                 * @param CallbackType Type of the callback
                 * @param ConnectionHandle Connection handle
                 * @param Data Data
                 * @param DataSize Size of the data
                 **/
                SDataListenerCallback(NsSmartDeviceLink::NsTransportManager::CTransportManager::EDataListenerCallbackType CallbackType, NsSmartDeviceLink::NsTransportManager::tConnectionHandle ConnectionHandle, const uint8_t* Data, size_t DataSize);

                /**
                 * @brief Constructor for creating FrameSendCompleted callback
                 *
                 * @param CallbackType Type of the callback
                 * @param ConnectionHandle Connection handle
                 * @param UserData User data
                 * @param SendStatus Status of operation
                 **/
                SDataListenerCallback(EDataListenerCallbackType CallbackType, tConnectionHandle ConnectionHandle, int UserData, ESendStatus SendStatus);

                /**
                 * @brief Copy constructor
                 *
                 * @param other Reference to object to be copied
                 **/
                SDataListenerCallback(const SDataListenerCallback& other);

                /**
                 * @brief Comparison operator.
                 *
                 * @param i_other Reference to the object to be compared with
                 * @return bool
                 **/
                bool operator==( const SDataListenerCallback& i_other ) const;

                /**
                 * @brief Destructor.
                 *
                 **/
                ~SDataListenerCallback(void);

                /**
                 * @brief Type of the callback that must be called
                 **/
                EDataListenerCallbackType mCallbackType;

                /**
                 * @brief Connection handle (will be sent in the ApplicationConnected and ApplicationDisconnected callbacks)
                 **/
                tConnectionHandle mConnectionHandle;

                /**
                 * @brief Data to send
                 **/
                uint8_t * mData;

                /**
                 * @brief Size of the data
                 **/
                size_t mDataSize;

                /**
                 * @brief User data
                 **/
                int mUserData;

                /**
                 * @brief Operation status
                 **/
                ESendStatus mSendStatus;
            };

            /**
             * @brief Struct that contains startup params for data-threads
             **/
            struct SDataThreadStartupParams
            {
                /**
                 * @brief Constructor
                 *
                 * @param TransportManager Pointer to the Transport Manager
                 * @param ConnectionHandle Connection Handle
                 **/
                SDataThreadStartupParams(NsSmartDeviceLink::NsTransportManager::CTransportManager* TransportManager, NsSmartDeviceLink::NsTransportManager::tConnectionHandle ConnectionHandle);

                /**
                 * @brief Pointer to Transport Manager
                 **/
                CTransportManager * mTransportManager;

                /**
                 * @brief Connection Handle
                 **/
                tConnectionHandle mConnectionHandle;
            };

            /**
             * @brief Incapsulates frame data for each connection
             * 
             **/
            struct SFrameDataForConnection
            {
                /**
                 * @brief Constructor
                 *
                 **/
                SFrameDataForConnection(tConnectionHandle ConnectionHandle);

                /**
                 * @brief Destructor
                 *
                 **/
                ~SFrameDataForConnection();

                /**
                 * @brief Copy constructor
                 *
                 * @param other Reference to object to be copied
                 **/
                SFrameDataForConnection(const SFrameDataForConnection& other);

                /**
                 * @brief Comparison operator.
                 *
                 * @param i_other Reference to the object to be compared with
                 * @return bool
                 **/
                bool operator==( const SFrameDataForConnection& i_other ) const;

                /**
                 * @brief Appends new data to the buffer
                 *
                 * @param Data Data to append
                 * @param DataSize Size of data
                 * @return void
                 **/
                void appendFrameData(const uint8_t * Data, size_t DataSize);

                /**
                 * @brief Extracts single frame and returns it
                 *
                 * @param[out] Data Pointer to the frame data
                 * @param[out] size_t Size of the returned frame.
                 *
                 * @return bool Result of extract operation
                 *
                 * @warning Memory for this buffer is allocated in the method call but must be freed
                 *          when it does not needed by caller component ONLY if method returns true
                 **/
                bool extractFrame(uint8_t *& Data, size_t & DataSize);

                /**
                 * @brief Pointer to the data buffer
                 **/
                uint8_t *mpDataBuffer;

                /**
                 * @brief Size of the data in the buffer
                 **/
                size_t mDataSize;

                /**
                 * @brief Size of the data buffer
                 **/
                size_t mBufferSize;

                /**
                 * @brief Handle of the connection
                 *
                 * @note Used only for debugging purposes
                 **/
                tConnectionHandle mConnectionHandle;

                /**
                 * @brief Logger
                 **/
                Logger mLogger;
            };

            /**
             * @brief Vector for storing Data Callbacks.
             **/
            typedef std::vector<SDataListenerCallback> tDataCallbacksVector;

            /**
             * @brief Struct for storing information, related to single connection
             **/
            struct SConnectionInfo
            {
                /**
                 * @brief Constructor.
                 *
                 * @param ConnectionHandle Connection handle.
                 * @param DeviceAdapter Device adapter
                 **/
                SConnectionInfo(const tConnectionHandle ConnectionHandle, IDeviceAdapter & DeviceAdapter);

                /**
                 * @brief Copy constructor
                 *
                 * @param other Reference to object to be copied
                 **/
                SConnectionInfo(const SConnectionInfo& other);

                /**
                 * @brief Comparison operator.
                 *
                 * @param i_other Reference to the object to be compared with
                 * @return bool
                 **/
                bool operator==( const SConnectionInfo& i_other ) const;

                /**
                 * @brief Destructor
                 *
                 **/
                virtual ~SConnectionInfo();

                /**
                 * @brief Connection handle, associated with connection information
                 **/
                const tConnectionHandle mConnectionHandle;

                /**
                 * @brief Terminate flag.
                 *
                 * This flag is set to notify connection thread that connection
                 * must be closed and connection thread must be terminated.
                 **/
                bool mTerminateFlag;

                /**
                 * @brief Callbacks for sending
                 **/
                tDataCallbacksVector mDataCallbacksVector;

                /**
                 * @brief Thread handle
                 **/
                pthread_t mConnectionThread;

                /**
                 * @brief Condition variable. Used for waiting for callbacks data
                 **/
                pthread_cond_t mConditionVar;

                /**
                 * @brief Device adapter, which opened connection
                 **/
                IDeviceAdapter& mpDeviceAdapter;

                /**
                 * @brief Frame data
                 **/
                SFrameDataForConnection mFrameData;
            };

            /**
             * @brief Connections map
             **/
            typedef std::map<tConnectionHandle, SConnectionInfo*> tConnectionsMap;

            /**
             * @brief Map for storing available devices for each device adapter
             **/
            typedef std::map<IDeviceAdapter*, tInternalDeviceList*> tDevicesByAdapterMap;

            /**
             * @brief Start routine for Application-related callbacks.
             *
             * @param Data Must be pointer to CTransportManager instance.
             *
             * @return Thread return value.
             **/
            static void * applicationCallbacksThreadStartRoutine(void * Data);

            /**
             * @brief Application-related callbacks thread.
             *
             * From this thread all application-related callbacks are called
             *
             **/
            void applicationCallbacksThread();

            /**
             * @brief Start routine for Data-related callbacks.
             *
             * @param Data Must be pointer to CTransportManager instance.
             *
             * @return Thread return value.
             **/
            static void * dataCallbacksThreadStartRoutine(void * Data);

            /**
             * @brief Data-related callbacks thread.
             *
             * From this thread all data-related callbacks related to given connection handle are called
             *
             * @param ConnectionHandle Connection handle
             **/
            void dataCallbacksThread(const tConnectionHandle ConnectionHandle);

            /**
             * @brief Starts thread for application-related callbacks
             * 
             **/
            void startApplicationCallbacksThread();

            /**
             * @brief Stops thread for application-related callbacks
             *
             * @return void
             * @warning terminate flag must be set to true before calling this function
             **/
            void stopApplicationCallbacksThread();

            /**
             * @brief Starts thread for data-related callbacks for given connection handle
             *
             * @param ConnectionHandle Connection Handle
             * @return bool
             **/
            void startConnection(const tConnectionHandle ConnectionHandle, IDeviceAdapter& DeviceAdapter);

            /**
             * @brief Stops thread for data-related callbacks for given connection handle
             *
             * @param ConnectionHandle Connection Handle
             * @return void
             * @warning terminate flag must be set to true before calling this function
             **/
            void stopConnection(const tConnectionHandle ConnectionHandle);

            /**
             * @brief Check thread existence for given ConnectionHandle
             *
             * @param ConnectionHandle Connection Handle
             * @return bool
             *
             * @attention This function is not thread safe
             **/
            bool isConnectionAvailable(const tConnectionHandle ConnectionHandle);

            /**
             * @brief Returns connection information by Connection Handle
             *
             * @param ConnectionHandle Connection handle
             * @return SConnectionInfo*
             **/
            SConnectionInfo* getConnection(const tConnectionHandle ConnectionHandle);

            /**
             * @brief Adds new device adapter
             *
             * @param DeviceAdapter Device adapter to add
             * @return void
             **/
            void addDeviceAdapter(NsSmartDeviceLink::NsTransportManager::IDeviceAdapter* DeviceAdapter);

            /**
             * @brief Removes device adapter
             *
             * @note This function does not deletes DeviceAdapter pointer
             *
             * @param DeviceAdapter Device adapter to remove
             * @return void
             **/
            void removeDeviceAdapter(NsSmartDeviceLink::NsTransportManager::IDeviceAdapter* DeviceAdapter);

            /**
             * @brief All device adapters initialized here
             *
             * @return void
             **/
            virtual void initializeDeviceAdapters();

            /**
             * @brief Sends callback DeviceListUpdated to subscribers
             *
             * @return void
             * @attention This function is not thread safe
             **/
            void sendDeviceListUpdatedCallback();

            /**
             * @brief Connects and disconnects device by its handle
             *
             * @param DeviceHandle Handle of the device to connect or disconnect
             * @param Connect Flag of device operation: true for connect, false for disconnect. Defaults to true.
             * @return void
             **/
            void connectDisconnectDevice(const NsSmartDeviceLink::NsTransportManager::tDeviceHandle DeviceHandle, bool Connect = true);

            /**
             * @brief Sends data callback
             *
             * @attention This function is not thread-safe
             * @param callback Callback to send
             * @return void
             **/
            void sendDataCallback(const SDataListenerCallback& callback);

            /**
             * @brief Sends device callback
             *
             * @param callback Callback to send
             * @return void
             **/
            void sendDeviceCallback(const SDeviceListenerCallback& callback);

            /**
             * @brief Device adapters.
             **/
            std::vector<IDeviceAdapter*> mDeviceAdapters;

            /**
             * @brief Logger.
             **/
            const log4cplus::Logger mLogger;

            /**
             * @brief Mutex restricting access to data listeners.
             **/
            mutable pthread_mutex_t mDataListenersMutex;

            /**
             * @brief Mutex restricting access to device listeners.
             **/
            mutable pthread_mutex_t mDeviceListenersMutex;

            /**
             * @brief Mutex restricting access to new device handle generation
             **/
            mutable pthread_mutex_t mDeviceHandleGenerationMutex;

            /**
             * @brief Mutex restricting access to new conenction handle generation
             **/
            mutable pthread_mutex_t mConnectionHandleGenerationMutex;

            /**
             * @brief Data listeners
             **/
            std::vector<ITransportManagerDataListener*> mDataListeners;

            /**
             * @brief Device listeners
             **/
            std::vector<ITransportManagerDeviceListener*> mDeviceListeners;

            /**
             * @brief Last used device handle
             *
             * Used during device handle generation
             **/
            tDeviceHandle mLastUsedDeviceHandle;

            /**
             * @brief Last used connection handle
             *
             * Used during connection handle generation
             **/
            tConnectionHandle mLastUsedConnectionHandle;

            /**
             * @brief ID of thread, which sends Application-related callbacks
             **/
            pthread_t mApplicationCallbacksThread;

            /**
             * @brief Condition variable used for device listeners callbacks synchronization
             **/
            pthread_cond_t mDeviceListenersConditionVar;

            /**
             * @brief Vector of the device listeners callbacks to call
             **/
            std::vector<SDeviceListenerCallback> mDeviceListenersCallbacks;

            /**
             * @brief Terminate flag.
             *
             * This flag is set to notify threads that they must be terminated.
             **/
            bool mTerminateFlag;

            /**
             * @brief Devices for each adapter
             **/
            tDevicesByAdapterMap mDevicesByAdapter;

            /**
             * @brief Mutex restricting access to devices information for each adapter
             **/
            mutable pthread_mutex_t mDevicesByAdapterMutex;

            /**
             * @brief Connections
             **/
            tConnectionsMap mConnections;

            /**
             * @brief Mutex restricting access to client interface
             **/
            mutable pthread_mutex_t mClientInterfaceMutex;
        };
    }
}

#endif
