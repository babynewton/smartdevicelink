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

#include "application_manager/commands/mobile/alert_request.h"

#include "application_manager/application_impl.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/message_helper.h"
#include <interfaces/HMI_API.h>

namespace application_manager {

namespace commands {

namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;

AlertRequest::AlertRequest(const MessageSharedPtr& message)
    : CommandRequestImpl(message),
      awaiting_ui_alert_response_(false),
      awaiting_tts_speak_response_(false),
      awaiting_tts_stop_speaking_response_(false),
      response_success_(false),
      flag_other_component_sent_(false),
      response_result_(mobile_apis::Result::SUCCESS),
      tts_speak_response_(mobile_apis::Result::INVALID_ENUM) {
  subscribe_on_event(hmi_apis::FunctionID::UI_OnResetTimeout);
}

AlertRequest::~AlertRequest() {
}

bool AlertRequest::Init() {

  /* Timeout in milliseconds.
     If omitted a standard value of 10000 milliseconds is used.*/
  if ((*message_)[strings::msg_params].keyExists(strings::duration)) {
    default_timeout_ =
        (*message_)[strings::msg_params][strings::duration].asUInt();
  } else {
    const int32_t def_value = 5000;
    default_timeout_ = def_value;
  }

  return true;
}

void AlertRequest::Run() {
  LOG4CXX_INFO(logger_, "AlertRequest::Run");

  uint32_t app_id = (*message_)[strings::params][strings::connection_key]
      .asInt();

  if (!Validate(app_id)) {
    // Invalid command, abort execution
    return;
  }

  awaiting_ui_alert_response_ = true;
  if ((*message_)[strings::msg_params].keyExists(strings::tts_chunks)) {
    if (0 < (*message_)[strings::msg_params][strings::tts_chunks].length()) {
      awaiting_tts_speak_response_ = true;
    }
  }
  SendAlertRequest(app_id);
  SendPlayToneNotification(app_id);
  if (awaiting_tts_speak_response_) {
    SendSpeakRequest(app_id);
  }
}

void AlertRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_INFO(logger_, "AlertRequest::on_event");
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::UI_OnResetTimeout: {
      LOG4CXX_INFO(logger_, "Received UI_OnResetTimeout event " << awaiting_tts_speak_response_ << " "
                   << awaiting_tts_stop_speaking_response_ << " " << awaiting_ui_alert_response_);
      ApplicationManagerImpl::instance()->updateRequestTimeout(
          connection_key(), correlation_id(), default_timeout());
      break;
    }
    case hmi_apis::FunctionID::UI_Alert: {
      LOG4CXX_INFO(logger_, "Received UI_Alert event");
      DCHECK(awaiting_ui_alert_response_);
      awaiting_ui_alert_response_ = false;

      if (awaiting_tts_speak_response_) {
        awaiting_tts_stop_speaking_response_ = true;
        SendHMIRequest(hmi_apis::FunctionID::TTS_StopSpeaking, NULL, true);
      }

      mobile_apis::Result::eType result_code =
          static_cast<mobile_apis::Result::eType>(
              message[strings::params][hmi_response::code].asInt());
      // Mobile Alert request is successful when UI_Alert is successful
      response_success_ = mobile_apis::Result::SUCCESS == result_code;
      if (!response_success_) {
        response_result_ = result_code;
      }
      response_params_ = message[strings::msg_params];
      break;
    }
    case hmi_apis::FunctionID::TTS_Speak: {
      LOG4CXX_INFO(logger_, "Received TTS_Speak event");
      DCHECK(awaiting_tts_speak_response_);
      awaiting_tts_speak_response_ = false;
      tts_speak_response_ = static_cast<mobile_apis::Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      break;
    }
    case hmi_apis::FunctionID::TTS_StopSpeaking: {
      LOG4CXX_INFO(logger_, "Received TTS_StopSpeaking event");
      DCHECK(awaiting_tts_stop_speaking_response_);
      awaiting_tts_stop_speaking_response_ = false;
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_,"Received unknown event" << event.id());
      return;
    }
  }
  if (!HasHmiResponsesToWait()) {
    std::string response_info("");
    if ((mobile_apis::Result::UNSUPPORTED_RESOURCE == tts_speak_response_) &&
        (!flag_other_component_sent_) && response_success_) {
      response_success_ = false;
      response_result_ = mobile_apis::Result::WARNINGS;
      response_info = "Unsupported phoneme type sent in a prompt";
    } else if ((mobile_apis::Result::UNSUPPORTED_RESOURCE ==
        tts_speak_response_) && (mobile_apis::Result::UNSUPPORTED_RESOURCE ==
            response_result_) && response_success_) {
      response_result_ = mobile_apis::Result::WARNINGS;
      response_info = "Unsupported phoneme type sent in a prompt and "
          "unsupported image sent in soft buttons";
    } else if ((mobile_apis::Result::UNSUPPORTED_RESOURCE ==
        tts_speak_response_) && response_success_) {
      response_result_ = mobile_apis::Result::WARNINGS;
      response_info = "Unsupported phoneme type sent in a prompt";
    }
    SendResponse(response_success_, response_result_,
                 response_info.empty() ? NULL : response_info.c_str(),
                 &response_params_);
  }
}

bool AlertRequest::Validate(uint32_t app_id) {
  ApplicationSharedPtr app = ApplicationManagerImpl::instance()->application(app_id);

  if (!app) {
    LOG4CXX_ERROR_EXT(logger_, "No application associated with session key");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return false;
  }

  mobile_apis::Result::eType processing_result =
      MessageHelper::ProcessSoftButtons((*message_)[strings::msg_params], app);

  if (mobile_apis::Result::SUCCESS != processing_result) {
    if (mobile_apis::Result::INVALID_DATA == processing_result) {
      LOG4CXX_ERROR(logger_, "INVALID_DATA!");
      SendResponse(false, processing_result);
      return false;
    }
    if (mobile_apis::Result::UNSUPPORTED_RESOURCE == processing_result) {
      LOG4CXX_ERROR(logger_, "UNSUPPORTED_RESOURCE!");
      response_result_ = processing_result;
    }
  }

  // check if mandatory params(alertText1 and TTSChunk) specified
  if ((!(*message_)[strings::msg_params].keyExists(strings::alert_text1))
      && (!(*message_)[strings::msg_params].keyExists(strings::alert_text2))
      && (!(*message_)[strings::msg_params].keyExists(strings::tts_chunks)
          && (1 > (*message_)[strings::msg_params]
                              [strings::tts_chunks].length()))) {
    LOG4CXX_ERROR_EXT(logger_, "Mandatory parameters are missing");
    SendResponse(false, mobile_apis::Result::INVALID_DATA,
                 "Mandatory parameters are missing");
    return false;
  }

  return true;
}

void AlertRequest::SendAlertRequest(int32_t app_id) {
  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);

  msg_params[hmi_request::alert_strings] = smart_objects::SmartObject(
      smart_objects::SmartType_Array);

  int32_t index = 0;
  if ((*message_)[strings::msg_params].keyExists(strings::alert_text1)) {
    msg_params[hmi_request::alert_strings][index][hmi_request::field_name] =
         TextFieldName::ALERT_TEXT1;
     msg_params[hmi_request::alert_strings][index][hmi_request::field_text] =
         (*message_)[strings::msg_params][strings::alert_text1];
     index++;
  }
  if ((*message_)[strings::msg_params].keyExists(strings::alert_text2)) {
    msg_params[hmi_request::alert_strings][index][hmi_request::field_name] =
        TextFieldName::ALERT_TEXT2;
    msg_params[hmi_request::alert_strings][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::alert_text2];
    index++;
  }
  if ((*message_)[strings::msg_params].keyExists(strings::alert_text3)) {
    msg_params[hmi_request::alert_strings][index][hmi_request::field_name] =
         TextFieldName::ALERT_TEXT3;
    msg_params[hmi_request::alert_strings][index][hmi_request::field_text] =
         (*message_)[strings::msg_params][strings::alert_text3];
  }

  // softButtons
  if ((*message_)[strings::msg_params].keyExists(strings::soft_buttons)) {
    msg_params[hmi_request::soft_buttons] =
        (*message_)[strings::msg_params][strings::soft_buttons];
  }
  // app_id
  msg_params[strings::app_id] = app_id;
  msg_params[strings::duration] = default_timeout_;

  // NAVI platform progressIndicator
  if ((*message_)[strings::msg_params].keyExists(strings::progress_indicator)) {
    msg_params[strings::progress_indicator] =
      (*message_)[strings::msg_params][strings::progress_indicator];
  }
  if (msg_params[hmi_request::alert_strings].length() > 0 ||
      msg_params.keyExists(hmi_request::soft_buttons)) {
    flag_other_component_sent_ = true;
  }
  SendHMIRequest(hmi_apis::FunctionID::UI_Alert, &msg_params, true);
}

void AlertRequest::SendSpeakRequest(int32_t app_id) {

  // crate HMI speak request
  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
    smart_objects::SmartType_Map);

  msg_params[hmi_request::tts_chunks] = smart_objects::SmartObject(
    smart_objects::SmartType_Array);
  msg_params[hmi_request::tts_chunks] =
    (*message_)[strings::msg_params][strings::tts_chunks];
  msg_params[strings::app_id] = app_id;
  SendHMIRequest(hmi_apis::FunctionID::TTS_Speak, &msg_params, true);
}

void AlertRequest::SendPlayToneNotification(int32_t app_id) {
  LOG4CXX_INFO(logger_, "AlertRequest::SendPlayToneNotification");

  // check playtone parameter
  if ((*message_)[strings::msg_params].keyExists(strings::play_tone)) {
    if ((*message_)[strings::msg_params][strings::play_tone].asBool()) {
      // crate HMI basic communication playtone request
      smart_objects::SmartObject msg_params = smart_objects::SmartObject(
          smart_objects::SmartType_Map);

      CreateHMINotification(hmi_apis::FunctionID::BasicCommunication_PlayTone,
                            msg_params);
    }
  }
}

bool AlertRequest::HasHmiResponsesToWait() {
  return awaiting_ui_alert_response_ || awaiting_tts_speak_response_
      || awaiting_tts_stop_speaking_response_;
}

}  // namespace commands

}  // namespace application_manager
