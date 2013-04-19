/**
* \file JSONHandler.hpp
* \brief JSONHandler class header.
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


#ifndef JSONHANDLER_CLASS
#define JSONHANDLER_CLASS 

#include "Logger.hpp"
#include "JSONHandler/MessageQueue.h"
#include "JSONHandler/SDLRPCMessage.h"
#include "JSONHandler/IRPCMessagesObserver.h"
#include "JSONHandler/IJsonHandler.h"
#include "ProtocolHandler/SmartDeviceLinkRawMessage.h"
#include "ProtocolHandler/IProtocolObserver.h"
#include "ProtocolHandler/ProtocolHandler.h"

const unsigned char RPC_REQUEST = 0x0;
const unsigned char RPC_RESPONSE = 0x1;
const unsigned char RPC_NOTIFICATION = 0x2;
const unsigned char RPC_UNKNOWN = 0xF;

/**
 * \class JSONHandler
 * \brief Class for handling message exchange between protocol layer and Application Manager.
 * Receives SmartDeviceLink Json message from Protocol layer, creates corresponding object and sends it to Application Manager.
 * Receives SmartDeviceLink message object from Application manager, serializes it into Json string and sends to Protocol Layer.
*/
class JSONHandler : public IJsonHandler, public NsProtocolHandler::IProtocolObserver
{
public:
    /**
     * \brief Constructor
     * \param protocolHandler Pointer to Protocol Layer handler for message exchange.
    */
    JSONHandler( NsProtocolHandler::ProtocolHandler * protocolHandler = 0 );

    /**
     * \brief Destructor
    */
    virtual ~JSONHandler();

    /*Methods from IProtocolObserver*/
    /**
     * \brief Sets pointer for Protocol layer handler for message exchange.
     * \param protocolHandler Pointer to Protocol layer handler.
     */
    void setProtocolHandler( NsProtocolHandler::ProtocolHandler * protocolHandler );

    /**
     * \brief Callback for Protocol layer handler to notify of message received.
     * \param message Object containing received data, size of it and connection key.
     */
    void onDataReceivedCallback( const NsProtocolHandler::SmartDeviceLinkRawMessage * message );
    /*end of methods from IProtocolObserver*/

    /*Methods for IRPCMessagesObserver*/
    /**
     * \brief Sets pointer to instance of the class implementing RPC handling (Application Manager).
     * \param messagesObserver Pointer to object of the class implementing IRPCMessagesObserver
     * \sa IRPCMessagesObserver
     */
    void setRPCMessagesObserver( IRPCMessagesObserver * messagesObserver );

    /**
     * \brief Method for sending message to Mobile Application. 
     * \param message Pointer to base class of SmartDeviceLink Json object
     * to be serialized to Json message and sent to mobile App.
     * \param sessionId ID of the session the message was received within.
     */
    void sendRPCMessage( const NsSmartDeviceLinkRPC::SDLRPCMessage * message, int connectionKey );
    /*End of methods for IRPCMessagesObserver*/
       
protected:
    /**
     * \brief Static method for handling messages from Mobile application.
     * \param params Pointer to JSONHandler instance.
     */
    static void * waitForIncomingMessages( void * params );

    /**
     * \brief Static method for handling messages to Mobile application.
     * \param params Pointer to JSONHandler instance.
     */
    static void * waitForOutgoingMessages( void * params );

    /**
     * \brief Helper method for clearing Json message from empty spaces
     * in order for it to be parsed correctly by Json library.
     * \param input Json string.
     * \return Json string cleared from empty spaces.
     */
    std::string clearEmptySpaces( const std::string & input );

    NsSmartDeviceLinkRPC::SDLRPCMessage * handleIncomingMessageProtocolV1(
            const NsProtocolHandler::SmartDeviceLinkRawMessage * message );

    NsSmartDeviceLinkRPC::SDLRPCMessage * handleIncomingMessageProtocolV2(
            const NsProtocolHandler::SmartDeviceLinkRawMessage * message );

    NsProtocolHandler::SmartDeviceLinkRawMessage * handleOutgoingMessageProtocolV1( 
            int connectionKey, const NsSmartDeviceLinkRPC::SDLRPCMessage *  message );

    NsProtocolHandler::SmartDeviceLinkRawMessage * handleOutgoingMessageProtocolV2( 
            int connectionKey, const NsSmartDeviceLinkRPC::SDLRPCMessage *  message );
    
private:
    /**
      *\brief For logging.
    */
    static log4cplus::Logger                     mLogger;

    /**
      *\brief Points on instance of class implementing RPC handling (Application Manager).
    */
    IRPCMessagesObserver *             mMessagesObserver;

    /* Data for IProtocolObserver */
    /**
      *\brief Points on instance of Protocol layer handler for message exchange.
    */
    NsProtocolHandler::ProtocolHandler *         mProtocolHandler;

    /* End IProtocolObserver data */

    /**
      *\brief Queue of messages from Mobile Application.
      *\sa MessageQueue
    */
    MessageQueue<const NsProtocolHandler::SmartDeviceLinkRawMessage*>          mIncomingMessages;

    /**
      *\brief Thread for handling messages from Mobile Application.
    */
    pthread_t             mWaitForIncomingMessagesThread;

    /**
      *\brief Queue of messages to Mobile Application.
      *\sa MessageQueue
    */
    MessageQueue<std::pair<int,const NsSmartDeviceLinkRPC::SDLRPCMessage*>>    mOutgoingMessages;

    /**
      *\brief Thread for handling messages to Mobile Application.
    */
    pthread_t             mWaitForOutgoingMessagesThread;

};

#endif  //  JSONHANDLER_CLASS
