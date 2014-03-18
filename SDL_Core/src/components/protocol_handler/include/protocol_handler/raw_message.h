/**
 * \file SmartDeviceLinkRawMessage.h
 * \brief SmartDeviceLinkRawMessage class header file.
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

#ifndef SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_RAW_MESSAGE_H_
#define SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_RAW_MESSAGE_H_

#include "utils/macro.h"
#include "utils/shared_ptr.h"
#include "protocol_handler/service_type.h"
#include "protocol_handler/message_priority.h"

/**
 *\namespace NsProtocolHandler
 *\brief Namespace for SmartDeviceLink ProtocolHandler related functionality.
 */

namespace protocol_handler {
/**
 * \class SmartDeviceLinkRawMessage
 * \brief Class-wraper for information about message for interchanging
 * between components.
 */
class RawMessage {
  public:
    /**
     * \brief Constructor
     * \param connectionKey Identifier of connection within wich message
     * is transferred
     * \param protocolVersion Version of protocol of the message
     * \param data Message string
     * \param dataSize Message size
     */
    RawMessage(int32_t connectionKey, uint32_t protocolVersion,
               uint8_t* data_param, uint32_t dataSize,
               uint8_t type = ServiceType::kRpc);

    /**
     * \brief Destructor
     */
    ~RawMessage();

    /**
     * \brief Getter for connection identifier
     */
    int32_t connection_key() const;

    void set_connection_key(uint32_t);

    /**
     * \brief Getter for message string
     */
    uint8_t* data() const;

    /**
     * \brief Getter for message size
     */
    uint32_t data_size() const;

    /**
     * \brief Getter for protocol version
     */
    uint32_t protocol_version() const;

    /**
     * \brief Getter for service type
     */
    ServiceType service_type() const {
      return service_type_;
    }

    bool IsWaiting() const;

    void set_waiting(bool v);

    /*
     * \brief Priority of this message based on it's service type
     */
    MessagePriority Priority() const;

  private:
    /**
     * \brief Connection Identifier
     * Obtained from \saCconnection_handler
     */
    int32_t connection_key_;

    /**
     * \brief Message string
     */
    uint8_t* data_;

    /**
     * \brief Size of message
     */
    uint32_t data_size_;

    /**
     * \brief Version of SmartDeviceLink protocol (currently 1,2)
     * used for tranferring message.
     */
    uint32_t protocol_version_;

    /**
     * \brief Type of service message belongs to
     */
    ServiceType service_type_;

    /**
     * specifies current state of message in queue. if false message is "ready to be processed"
     * otherwise it is "waiting for response"
     *
     */
    bool waiting_;

    /**
     * \brief Specifies if this is binary data
     * (used by ProtocolObserver).
     */
    bool fully_binary_;

    /**
     * \brief Id of connection (for service messages like start/end session)
     *
     */

    DISALLOW_COPY_AND_ASSIGN(RawMessage);
};

typedef  utils::SharedPtr<RawMessage> RawMessagePtr;
}  // namespace protocol_handler

#endif  // SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_RAW_MESSAGE_H_
