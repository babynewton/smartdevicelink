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
#include "application_manager/commands/mobile/show_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application.h"
#include "application_manager/message_helper.h"
#include "utils/file_system.h"

namespace application_manager {

namespace commands {

ShowRequest::ShowRequest(const MessageSharedPtr& message)
 : CommandRequestImpl(message),
   result_(mobile_apis::Result::INVALID_ENUM) {
}

ShowRequest::~ShowRequest() {
}

void ShowRequest::Run() {
  LOG4CXX_INFO(logger_, "ShowRequest::Run");

  ApplicationSharedPtr app = application_manager::ApplicationManagerImpl::instance()
      ->application(
      (*message_)[strings::params][strings::connection_key].asInt());

  if (!app) {
    LOG4CXX_ERROR_EXT(
        logger_, "An application " << app->name() << " is not registered.");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
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
    if (mobile_apis::Result::INVALID_DATA == verification_result) {
      LOG4CXX_ERROR(logger_, "VerifyImageFiles INVALID_DATA!");
      SendResponse(false, verification_result);
      return;
    }
    if (mobile_apis::Result::UNSUPPORTED_RESOURCE == verification_result) {
      LOG4CXX_ERROR(logger_, "VerifyImageFiles UNSUPPORTED_RESOURCE!");
      result_ = verification_result;
    }
  }

  if (!CheckMenuFieldsNames()) {
    LOG4CXX_ERROR(logger_, "Bad menu name");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }
  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);
  msg_params[strings::app_id] = app->app_id();

  msg_params[hmi_request::show_strings] = smart_objects::SmartObject(
      smart_objects::SmartType_Array);

  int32_t index = 0;
  if ((*message_)[strings::msg_params].keyExists(strings::main_field_1)) {
    msg_params[hmi_request::show_strings][index][hmi_request::field_name] =
        static_cast<int32_t>(application_manager::TextFieldName::MAIN_FILED1);
    msg_params[hmi_request::show_strings][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::main_field_1];
    ++index;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::main_field_2)) {
    msg_params[hmi_request::show_strings][index][hmi_request::field_name] =
        static_cast<int32_t>(application_manager::TextFieldName::MAIN_FILED2);
    msg_params[hmi_request::show_strings][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::main_field_2];
    ++index;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::main_field_3)) {
    msg_params[hmi_request::show_strings][index][hmi_request::field_name] =
        static_cast<int32_t>(application_manager::TextFieldName::MAIN_FILED3);
    msg_params[hmi_request::show_strings][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::main_field_3];
    ++index;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::main_field_4)) {
    msg_params[hmi_request::show_strings][index][hmi_request::field_name] =
        static_cast<int32_t>(application_manager::TextFieldName::MAIN_FILED4);
    msg_params[hmi_request::show_strings][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::main_field_4];
    ++index;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::media_clock)) {
    msg_params[hmi_request::show_strings][index][hmi_request::field_name] =
        static_cast<int32_t>(application_manager::TextFieldName::MEDIA_CLOCK);
    msg_params[hmi_request::show_strings][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::media_clock];
    ++index;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::media_track)) {
    msg_params[hmi_request::show_strings][index][hmi_request::field_name] =
        static_cast<int32_t>(application_manager::TextFieldName::MEDIA_TRACK);
    msg_params[hmi_request::show_strings][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::media_track];
    ++index;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::status_bar)) {
    msg_params[hmi_request::show_strings][index][hmi_request::field_name] =
        static_cast<int32_t>(application_manager::TextFieldName::STATUS_BAR);
    msg_params[hmi_request::show_strings][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::status_bar];
    ++index;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::alignment)) {
    msg_params[strings::alignment] =
        (*message_)[strings::msg_params][strings::alignment];
  }

  if ((*message_)[strings::msg_params].keyExists(strings::graphic)) {
    msg_params[strings::graphic] =
        (*message_)[strings::msg_params][strings::graphic];
  }

  if ((*message_)[strings::msg_params].keyExists(strings::secondary_graphic)) {
    msg_params[strings::secondary_graphic] =
            (*message_)[strings::msg_params][strings::secondary_graphic];
  }

  if ((*message_)[strings::msg_params].keyExists(strings::soft_buttons)) {
    msg_params[strings::soft_buttons] =
        (*message_)[strings::msg_params][strings::soft_buttons];
  }

  if ((*message_)[strings::msg_params].keyExists(strings::custom_presets)) {
    msg_params[strings::custom_presets] =
        (*message_)[strings::msg_params][strings::custom_presets];
  } else {
    HMICapabilities& hmi_capabilities =
        ApplicationManagerImpl::instance()->hmi_capabilities();
    const smart_objects::SmartObject* display_capabilities =
        hmi_capabilities.display_capabilities();
    msg_params[strings::custom_presets] = smart_objects::SmartObject(
        smart_objects::SmartType_Array);
    int amount_custom_preset = 0;
    if ((*display_capabilities).keyExists
        (hmi_response::num_custom_presets_available)) {
      amount_custom_preset = (*display_capabilities)
          [hmi_response::num_custom_presets_available].asInt();
    }
    char name_custom_preset [11] = {0};
    for (int i = 0; i < amount_custom_preset; ++i) {
      snprintf(name_custom_preset, sizeof(name_custom_preset),
              "%s%d", "PRESET_", i+1);
      msg_params[strings::custom_presets][i] = name_custom_preset;
    }
  }

  SendHMIRequest(hmi_apis::FunctionID::UI_Show, &msg_params, true);

  MessageSharedPtr persistentData = new smart_objects::SmartObject(msg_params);
  app->set_show_command(*persistentData);
}

void ShowRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_INFO(logger_, "ShowRequest::on_event");
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::UI_Show: {
      LOG4CXX_INFO(logger_, "Received UI_Show event");

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

bool ShowRequest::CheckMenuFieldsNames() {
  if ((*message_)[strings::msg_params].keyExists(strings::main_field_4)) {
    const std::string& str = (*message_)[strings::msg_params]
                                         [strings::main_field_4].asString();
    if (!CheckSyntax(str, true)) {
      LOG4CXX_INFO(logger_, "main_field_4 syntax check failed");
      return  false;
    }
  }
  if ((*message_)[strings::msg_params].keyExists(strings::main_field_3)) {
    const std::string& str = (*message_)[strings::msg_params]
                                         [strings::main_field_3].asString();
    if (!CheckSyntax(str, true)) {
      LOG4CXX_INFO(logger_, "main_field_3 syntax check failed");
      return false;
    }
  }
  if ((*message_)[strings::msg_params].keyExists(strings::main_field_2)) {
    const std::string& str = (*message_)[strings::msg_params]
                                         [strings::main_field_2].asString();
    if (!CheckSyntax(str, true)) {
      LOG4CXX_INFO(logger_, "main_field_2 syntax check failed");
      return false;
    }
  }
  if ((*message_)[strings::msg_params].keyExists(strings::main_field_1)) {
    const std::string& str = (*message_)[strings::msg_params]
                                         [strings::main_field_1].asString();
    if (!CheckSyntax(str, true)) {
      LOG4CXX_INFO(logger_, "main_field_1 syntax check failed");
      return false;
    }
  }
  return true;
}

}  // namespace commands

}  // namespace application_manager
