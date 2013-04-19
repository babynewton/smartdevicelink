/**
* \file ProtocolHandler.h
* \brief ProtocolHandler class header file.
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

#ifndef PROTOCOLHANDLER_CLASS
#define PROTOCOLHANDLER_CLASS

#include <map>
#include "Logger.hpp"
#include "Utils/MessageQueue.h"
#include "ProtocolHandler/SmartDeviceLinkRawMessage.h"
#include "ProtocolHandler/ProtocolPacket.h"
#include "TransportManager/ITransportManagerDataListener.hpp"

/**
  *\namespace NsProtocolHandler
  *\brief Namespace for SmartDeviceLink ProtocolHandler related functionality.
*/
namespace NsProtocolHandler
{
    class IProtocolObserver;
    class ISessionObserver;  

    /**
     * \class ProtocolHandler
     * \brief Class for handling message exchange between Transport and higher layers.
     * Receives message in form of array of bytes, parses its protocol, handles according to 
     * parsing results (version number, start/end session etc) and if needed passes message 
     * to JSON Handler or notifies Connection Handler about activities around sessions.
    */
    class ProtocolHandler : public NsSmartDeviceLink::NsTransportManager::ITransportManagerDataListener
    {
    public:
        /**
         * \brief Constructor
         * \param transportManager Pointer to Transport layer handler for message exchange.
        */
        ProtocolHandler( NsSmartDeviceLink::NsTransportManager::ITransportManager * transportManager );

        /**
         * \brief Destructor
        */
        ~ProtocolHandler();

        /**
         * \brief Sets pointer for higher layer handler for message exchange
         * \param observer Pointer to object of the class implementing IProtocolObserver
         */
        void setProtocolObserver( IProtocolObserver * observer );

        /**
         * \brief Sets pointer for Connection Handler layer for managing sessions
         * \param observer Pointer to object of the class implementing ISessionObserver
         */
        void setSessionObserver( ISessionObserver * observer );

        /**
         * \brief Method for sending message to Mobile Application.
         * \param message Message with params to be sent to Mobile App.
         */
        void sendData(const SmartDeviceLinkRawMessage * message);
    
    protected:

        /**
         * \brief Static method for handling message from Transport Layer.
         * \param params Pointer to instance of ProtocolHandler class.
         */
        static void * handleMessagesFromMobileApp( void * params );

        /**
         * \brief Static method for handling message to Transport Layer.
         * \param params Pointer to instance of ProtocolHandler class.
         */
        static void * handleMessagesToMobileApp( void * params );

        /**
         * \brief Sends fail of ending session to mobile application.
         * \param connectionHandle Identifier of connection whithin which session exists
         * \param sessionID ID of session ment to be ended
         * \param serviceType Type of session: RPC or BULK Data. RPC by default.
         */
        void sendEndSessionNAck( NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
              unsigned int sessionID,
              unsigned char serviceType = SERVICE_TYPE_RPC );

        /**
         * \brief Sends acknowledgement of starting session to mobile application
         * with session number and hash code for second version of protocol.
         * \param connectionHandle Identifier of connection whithin which session was started
         * \param sessionID ID of session to be sent to mobile applicatin.
         * \param protocolVersion Version of protocol used for communication
         * \param hashCode For second version of protocol: identifier of session to be sent to 
         * mobile app for using when ending session.
         * \param serviceType Type of session: RPC or BULK Data. RPC by default.
         */
        void sendStartSessionAck( NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
              unsigned char sessionID,
              unsigned char protocolVersion,
              unsigned int hashCode = 0,
              unsigned char serviceType = SERVICE_TYPE_RPC );

        /**
         * \brief Sends fail of starting session to mobile application.
         * \param connectionHandle Identifier of connection whithin which session ment to be started
         * \param serviceType Type of session: RPC or BULK Data. RPC by default.
         */
        void sendStartSessionNAck( NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
              unsigned char serviceType = SERVICE_TYPE_RPC );

    private:
        /**
         * \struct IncomingMessage
         * \brief Used for storing both message and info about message: 
         * its size and connection through which it's handled.
        */
        struct IncomingMessage
        {
          /**
           * \brief Identifier of connection through which message is transported.
           */
          NsSmartDeviceLink::NsTransportManager::tConnectionHandle mConnectionHandle;
          /**
           * \brief Message string.
           */
          unsigned char * mData;
          /**
           * \brief Size of message.
           */
          unsigned int mDataSize;
        };

        /**
         * @brief Processing frame received callbacks.
         *
         * @param ConnectionHandle Connection handle.
         * @param Data Received frame payload data.
         * @param DataSize Size of data in bytes.
        **/
        virtual void onFrameReceived(NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
            const uint8_t * data, size_t dataSize);

        /**
         * @brief Frame send completed callback.
         *
         * @param ConnectionHandle Connection handle.
         * @param UserData User data that was previously passed to ITransportManager::sendFrame.
         * @param SendStatus Result status.
         **/
        virtual void onFrameSendCompleted(NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
            int userData, NsSmartDeviceLink::NsTransportManager::ESendStatus sendStatus);
    
        /**
         * \brief Sends message which size permits to send it in one frame.
         * \param connectionHandle Identifier of connection through which message is to be sent.
         * \param sessionID ID of session through which message is to be sent.
         * \param protocolVersion Version of Protocol used in message.
         * \param servType Type of session, RPC or BULK Data
         * \param dataSize Size of message excluding protocol header
         * \param data Message string
         * \param compress Compression flag
         * \return \saRESULT_CODE Status of operation
         */
        RESULT_CODE sendSingleFrameMessage(NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
                                      const unsigned char sessionID,
                                      unsigned int protocolVersion,
                                      const unsigned char servType,
                                      const unsigned int dataSize,
                                      const unsigned char *data,
                                      const bool compress);

        /**
         * \brief Sends message which size doesn't permit to send it in one frame.
         * \param connectionHandle Identifier of connection through which message is to be sent.
         * \param sessionID ID of session through which message is to be sent.
         * \param protocolVersion Version of Protocol used in message.
         * \param servType Type of session, RPC or BULK Data
         * \param dataSize Size of message excluding protocol header
         * \param data Message string
         * \param compress Compression flag
         * \param maxDataSize Maximum allowed size of single frame.
         * \return \saRESULT_CODE Status of operation
         */
        RESULT_CODE sendMultiFrameMessage(NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
                                         const unsigned char sessionID,
                                         unsigned int protocolVersion,
                                         const unsigned char servType,
                                         const unsigned int dataSize,
                                         const unsigned char *data,
                                         const bool compress,
                                         const unsigned int maxDataSize);
        
        /**
         * \brief Sends message already containing protocol header.
         * \param connectionHandle Identifier of connection through which message is to be sent.
         * \param packet Message with protocol header.
         * \return \saRESULT_CODE Status of operation
         */
        RESULT_CODE sendFrame( NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
              const ProtocolPacket & packet );

        /**
         * \brief Handles received message.
         * \param connectionHandle Identifier of connection through which message is received.
         * \param packet Received message with protocol header.
         * \return \saRESULT_CODE Status of operation
         */
        RESULT_CODE handleMessage( NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
              ProtocolPacket * packet );

        /**
         * \brief Handles message received in multiple frames. Collects all frames of message.
         * \param connectionHandle Identifier of connection through which message is received.
         * \param packet Current frame of message with protocol header. 
         * \return \saRESULT_CODE Status of operation
         */
        RESULT_CODE handleMultiFrameMessage( NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
              ProtocolPacket * packet );

        /**
         * \brief Handles message received in single frame.
         * \param connectionHandle Identifier of connection through which message is received.
         * \param packet Received message with protocol header. 
         * \return \saRESULT_CODE Status of operation
         */
        RESULT_CODE handleControlMessage( NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
              const ProtocolPacket * packet );
        
        /**
         * \brief For logging.
         */
        static log4cplus::Logger mLogger;

        /**
          *\brief Pointer on instance of class implementing IProtocolObserver (JSON Handler)
        */
        IProtocolObserver * mProtocolObserver;

        /**
          *\brief Pointer on instance of class implementing ISessionObserver (Connection Handler)
        */
        ISessionObserver * mSessionObserver;

        /**
          *\brief Pointer on instance of Transport layer handler for message exchange.
        */
        NsSmartDeviceLink::NsTransportManager::ITransportManager * mTransportManager;

        /**
          *\brief Queue for message from Mobile side.
        */
        MessageQueue<IncomingMessage *> mMessagesFromMobileApp;

        /**
          *\brief Thread for handling messages from Mobile side.
        */
        pthread_t mHandleMessagesFromMobileApp;

        /**
          *\brief Queue for message to Mobile side.
        */
        MessageQueue<const SmartDeviceLinkRawMessage *> mMessagesToMobileApp;

        /**
          *\brief Thread for handling message to Mobile side.
        */
        pthread_t mHandleMessagesToMobileApp;

        /**
          *\brief Map of frames for messages received in multiple frames.
        */
        std::map<int, ProtocolPacket *> mIncompleteMultiFrameMessages;

        /**
          *\brief Counter of messages sent in each session.
        */
        std::map<unsigned char, unsigned int> mMessageCounters;
    };
}

#endif  //  PROTOCOLHANDLER_CLASS
