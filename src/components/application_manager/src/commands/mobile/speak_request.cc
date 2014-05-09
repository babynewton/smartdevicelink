/*

 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include "application_manager/commands/mobile/speak_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"

namespace application_manager {

namespace commands {

SpeakRequest::SpeakRequest(const MessageSharedPtr& message)
  : CommandRequestImpl(message) {
}

SpeakRequest::~SpeakRequest() {
}

void SpeakRequest::Run() {
  LOG4CXX_INFO(logger_, "SpeakRequest::Run");

  ApplicationSharedPtr app = application_manager::ApplicationManagerImpl::instance()
                     ->application(connection_key());

  if (!app) {
    LOG4CXX_ERROR_EXT(logger_, "NULL pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  (*message_)[strings::msg_params][strings::app_id] = app->app_id();
  SendHMIRequest(hmi_apis::FunctionID::TTS_Speak,
                 &message_->getElement(strings::msg_params), true);
}

void SpeakRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_INFO(logger_, "SpeakRequest::on_event");
  switch (event.id()) {
    case hmi_apis::FunctionID::TTS_Speak: {
      LOG4CXX_INFO(logger_, "Received TTS_Speak event");

      ProcessTTSSpeakResponse(event.smart_object());
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
      break;
    }
  }
}

void SpeakRequest::ProcessTTSSpeakResponse(
  const smart_objects::SmartObject& message) {
  LOG4CXX_INFO(logger_, "SpeakRequest::ProcessTTSSpeakResponse");
  ApplicationSharedPtr application = ApplicationManagerImpl::instance()->application(
                               connection_key());

  if (!application) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    return;
  }

  bool result = false;
  mobile_apis::Result::eType result_code =
    static_cast<mobile_apis::Result::eType>(
      message[strings::params][hmi_response::code].asInt());
  if (hmi_apis::Common_Result::SUCCESS ==
      static_cast<hmi_apis::Common_Result::eType>(result_code)) {
    result = true;
  }
  (*message_)[strings::params][strings::function_id] =
    mobile_apis::FunctionID::SpeakID;

  const char* return_info = NULL;

  if (result) {
      if (hmi_apis::Common_Result::UNSUPPORTED_RESOURCE ==
          static_cast<hmi_apis::Common_Result::eType>(result_code)) {
        result_code = mobile_apis::Result::WARNINGS;
        return_info = std::string("Unsupported phoneme type sent in a prompt").c_str();
      }
  }

  SendResponse(result, static_cast<mobile_apis::Result::eType>(result_code),
               return_info, &(message[strings::msg_params]));
}

}  // namespace commands

}  // namespace application_manager
