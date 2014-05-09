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

#include "application_manager/commands/mobile/perform_audio_pass_thru_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"

namespace application_manager {

namespace commands {

namespace str = strings;

PerformAudioPassThruRequest::PerformAudioPassThruRequest(
    const MessageSharedPtr& message)
    : CommandRequestImpl(message) {
}

PerformAudioPassThruRequest::~PerformAudioPassThruRequest() {
}

bool PerformAudioPassThruRequest::Init() {
  default_timeout_ += (*message_)[str::msg_params][str::max_duration].asInt();
  return true;
}

void PerformAudioPassThruRequest::Run() {
  LOG4CXX_INFO(logger_, "PerformAudioPassThruRequest::Run");

  if (ApplicationManagerImpl::instance()->begin_audio_pass_thru() == false) {
    // Audio pass thru already active
    LOG4CXX_ERROR_EXT(logger_, "REJECTED");
    SendResponse(false, mobile_apis::Result::REJECTED);
    return;
  }

  uint32_t app_id = (*message_)[strings::params][strings::connection_key]
      .asUInt();
  ApplicationSharedPtr app = ApplicationManagerImpl::instance()->application(app_id);

  if (!app) {
    LOG4CXX_ERROR_EXT(logger_, "APPLICATION_NOT_REGISTERED");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (mobile_api::HMILevel::HMI_NONE == app->hmi_level()) {
    LOG4CXX_ERROR_EXT(logger_, "application isn't activated");
    SendResponse(false, mobile_apis::Result::REJECTED);
    return;
  }

  SendSpeakRequest(app->app_id());

  // create HMI request
  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);

  msg_params[str::app_id] = app_id;

  msg_params[hmi_request::audio_pass_display_texts] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  if ((*message_)[str::msg_params].keyExists(str::audio_pass_display_text1)) {
    msg_params[hmi_request::audio_pass_display_texts]
               [0][hmi_request::field_name] = static_cast<int32_t>
    (application_manager::TextFieldName::AUDIO_DISPLAY_TEXT1);
    msg_params[hmi_request::audio_pass_display_texts]
               [0][hmi_request::field_text] =
        (*message_)[str::msg_params][str::audio_pass_display_text1];
  }

  if ((*message_)[str::msg_params].keyExists(str::audio_pass_display_text2)) {
    msg_params[hmi_request::audio_pass_display_texts]
               [1][hmi_request::field_name] = static_cast<int32_t>
    (application_manager::TextFieldName::AUDIO_DISPLAY_TEXT2);
    msg_params[hmi_request::audio_pass_display_texts]
               [1][hmi_request::field_text] =
        (*message_)[str::msg_params][str::audio_pass_display_text2];
  }

  // duration
  msg_params[hmi_request::max_duration] =
      (*message_)[str::msg_params][str::max_duration];

  msg_params[strings::app_id] = app->app_id();

  SendHMIRequest(hmi_apis::FunctionID::UI_PerformAudioPassThru,
                 &msg_params, true);

  ApplicationManagerImpl::instance()->StartAudioPassThruThread(
      (*message_)[str::params][str::connection_key].asInt(),
      (*message_)[str::params][str::correlation_id].asInt(),
      (*message_)[str::msg_params][str::max_duration].asInt(),
      (*message_)[str::msg_params][str::sampling_rate].asInt(),
      (*message_)[str::msg_params][str::bits_per_sample].asInt(),
      (*message_)[str::msg_params][str::audio_type].asInt());
}

void PerformAudioPassThruRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_INFO(logger_, "PerformAudioPassThruRequest::on_event");
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::UI_PerformAudioPassThru: {
      mobile_apis::Result::eType result_code =
          static_cast<mobile_apis::Result::eType>(
              message[strings::params][hmi_response::code].asInt());

      bool result = mobile_apis::Result::SUCCESS == result_code ||
                    mobile_apis::Result::RETRY == result_code;

      if (ApplicationManagerImpl::instance()->end_audio_pass_thru()) {
        int32_t session_key =
          (*message_)[strings::params][strings::connection_key].asUInt();
        ApplicationManagerImpl::instance()->StopAudioPassThru(session_key);
      }

      const char* return_info = NULL;

      if (result) {
        if (hmi_apis::Common_Result::UNSUPPORTED_RESOURCE ==
            static_cast<hmi_apis::Common_Result::eType>(result_code)) {
          result_code = mobile_apis::Result::WARNINGS;
          return_info = std::string("Unsupported phoneme type sent in a prompt").c_str();
        }
      }

      SendResponse(result, result_code, return_info, &(message[strings::msg_params]));
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
      return;
    }
  }
}

void PerformAudioPassThruRequest::SendSpeakRequest(const int32_t app_id) {
  // crate HMI TTS speak request
  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);


  if ((*message_)[str::msg_params].keyExists(str::initial_prompt) &&
      (0 < (*message_)[str::msg_params][str::initial_prompt].length())) {
    for (int32_t i = 0;
        i < (*message_)[str::msg_params][str::initial_prompt].length();
        ++i) {
      msg_params[hmi_request::tts_chunks][i][str::text] =
          (*message_)[str::msg_params][str::initial_prompt][i][str::text];
    }
    // app_id
    msg_params[strings::app_id] = app_id;

    SendHMIRequest(hmi_apis::FunctionID::TTS_Speak, &msg_params);
  }
}

}  // namespace commands

}  // namespace application_manager
