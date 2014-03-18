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

#include <string>
#include "application_manager/commands/mobile/update_turn_list_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace commands {

UpdateTurnListRequest::UpdateTurnListRequest(const MessageSharedPtr& message)
 : CommandRequestImpl(message),
   result_(mobile_apis::Result::INVALID_ENUM) {
}

UpdateTurnListRequest::~UpdateTurnListRequest() {
}

void UpdateTurnListRequest::Run() {
  LOG4CXX_INFO(logger_, "UpdateTurnListRequest::Run");

  ApplicationSharedPtr app = ApplicationManagerImpl::instance()->application(
      (*message_)[strings::params][strings::connection_key].asUInt());

  if (!app) {
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    LOG4CXX_ERROR(logger_, "Application is not registered");
    return;
  }

  mobile_apis::Result::eType processing_result =
      MessageHelper::ProcessSoftButtons((*message_)[strings::msg_params], app);

  if (mobile_apis::Result::SUCCESS != processing_result) {
    if (mobile_apis::Result::INVALID_DATA == processing_result) {
      LOG4CXX_ERROR(logger_, "INVALID_DATA!");
      SendResponse(false, processing_result);
      return;
    }
    if (mobile_apis::Result::UNSUPPORTED_RESOURCE == processing_result) {
      LOG4CXX_ERROR(logger_, "UNSUPPORTED_RESOURCE!");
      result_ = processing_result;
    }
  }

  mobile_apis::Result::eType verification_result =
      MessageHelper::VerifyImageFiles((*message_)[strings::msg_params], app);

  if (mobile_apis::Result::SUCCESS != verification_result) {
    LOG4CXX_ERROR_EXT(
        logger_,
        "MessageHelper::VerifyImageFiles return " << verification_result);
    SendResponse(false, verification_result);
    return;
  }

  if (!CheckTurnListArray()) {
    LOG4CXX_ERROR(logger_, "INVALID_DATA!");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }


  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);
  msg_params = (*message_)[strings::msg_params];
  for (uint32_t i = 0; i < msg_params[strings::turn_list].length(); ++i) {
    if (msg_params[strings::turn_list][i].keyExists(hmi_request::navi_text)) {
      std::string navigation_text =
          msg_params[strings::turn_list][i][hmi_request::navi_text].asString();
      msg_params[strings::turn_list][i].erase(hmi_request::navi_text);
      msg_params[strings::turn_list]
                 [i][hmi_request::navi_text][hmi_request::field_name] =
          static_cast<int>(hmi_apis::Common_TextFieldName::turnText);
      msg_params[strings::turn_list]
                 [i][hmi_request::navi_text][hmi_request::field_text] =
          navigation_text;
    }
  }

  msg_params[strings::app_id] = app->app_id();


  SendHMIRequest(hmi_apis::FunctionID::Navigation_UpdateTurnList, &msg_params,
                   true);
}

void UpdateTurnListRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_INFO(logger_, "UpdateTurnListRequest::on_event");
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::Navigation_UpdateTurnList: {
      LOG4CXX_INFO(logger_, "Received Navigation_UpdateTurnList event");

      mobile_apis::Result::eType result_code =
          static_cast<mobile_apis::Result::eType>(
          message[strings::params][hmi_response::code].asInt());

      bool result = mobile_apis::Result::SUCCESS == result_code;
      if (mobile_apis::Result::INVALID_ENUM != result_) {
        result_code = result_;
      }

      SendResponse(result, result_code, NULL, &(message[strings::msg_params]));
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_,"Received unknown event" << event.id());
      break;
    }
  }
}

bool UpdateTurnListRequest::CheckTurnListArray() {
  if (!(*message_)[strings::msg_params].keyExists(strings::turn_list)) {
    return false;
  }

  int32_t length = (*message_)[strings::msg_params][strings::turn_list].length();
  if (0 == length) {
    return false;
  }
  for (int32_t i = 0; i < length; ++i) {
    if (!((*message_)[strings::msg_params][strings::turn_list][i].
        keyExists(hmi_request::navi_text)) &&
        !((*message_)[strings::msg_params][strings::turn_list][i].
        keyExists(strings::turn_icon))) {
      return false;
    }
  }
  return true;
}

}  // namespace commands

}  // namespace application_manager
