/**
 * \file SmartDeviceLinkRawMessage.cpp
 * \brief SmartDeviceLinkRawMessage class source file.
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

#include "protocol_handler/raw_message.h"

#include "protocol_handler/message_priority.h"

namespace protocol_handler {

RawMessage::RawMessage(int32_t connectionKey, uint32_t protocolVersion,
                       uint8_t* data_param, uint32_t data_sz,
                       uint8_t type)
  : connection_key_(connectionKey),
    protocol_version_(protocolVersion),
    service_type_(ServiceTypeFromByte(type)),
    waiting_(false),
    fully_binary_(false),
    data_size_(data_sz) {
  if (data_sz > 0) {
    data_ = new uint8_t[data_sz];
    for (uint32_t i = 0; i < data_sz; ++i) {
      data_[i] = data_param[i];
    }
  } else {
    data_ = 0;
  }
}

RawMessage::~RawMessage() {
  if (data_) {
    delete[] data_;
    data_ = 0;
  }
}

int32_t RawMessage::connection_key() const {
  return connection_key_;
}

void RawMessage::set_connection_key(uint32_t key) {
  connection_key_ = key;
}

uint8_t* RawMessage::data() const {
  return data_;
}

uint32_t RawMessage::data_size() const {
  return data_size_;
}

uint32_t RawMessage::protocol_version() const {
  return protocol_version_;
}

bool RawMessage::IsWaiting() const {
  return waiting_;
}

void RawMessage::set_waiting(bool v) {
  waiting_ = v;
}

MessagePriority RawMessage::Priority() const {
  return MessagePriority::FromServiceType(this->service_type());
}

}  // namespace protocol_handler
