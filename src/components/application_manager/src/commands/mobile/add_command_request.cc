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
#include "application_manager/commands/mobile/add_command_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application.h"
#include "application_manager/message_helper.h"
#include "utils/file_system.h"

namespace application_manager {

namespace commands {

AddCommandRequest::AddCommandRequest(const MessageSharedPtr& message)
    : CommandRequestImpl(message),
      is_ui_send_(false),
      is_vr_send_(false),
      is_ui_received_(false),
      is_vr_received_(false),
      ui_result_(hmi_apis::Common_Result::INVALID_ENUM),
      vr_result_(hmi_apis::Common_Result::INVALID_ENUM)  {
}

AddCommandRequest::~AddCommandRequest() {
}

void AddCommandRequest::Run() {
  LOG4CXX_INFO(logger_, "AddCommandRequest::Run");

  ApplicationSharedPtr app = ApplicationManagerImpl::instance()->application(
      (*message_)[strings::params][strings::connection_key].asUInt());

  if (!app) {
    LOG4CXX_ERROR_EXT(logger_, "No application associated with session key");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
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

  if (!((*message_)[strings::msg_params].keyExists(strings::cmd_id))) {
    LOG4CXX_ERROR_EXT(logger_, "INVALID_DATA");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  if (app->FindCommand(
      (*message_)[strings::msg_params][strings::cmd_id].asUInt())) {
    LOG4CXX_ERROR_EXT(logger_, "INVALID_ID");
    SendResponse(false, mobile_apis::Result::INVALID_ID);
    return;
  }

  bool data_exist =  false;

  if ((*message_)[strings::msg_params].keyExists(strings::menu_params)) {
    if (!CheckCommandName(app)) {
      SendResponse(false, mobile_apis::Result::DUPLICATE_NAME);
      return;
    }
    if ((*message_)[strings::msg_params][strings::menu_params].keyExists(
        hmi_request::parent_id)) {
      if (!CheckCommandParentId(app)) {
        SendResponse(false, mobile_apis::Result::INVALID_ID,
                     "Parent ID doesn't exist");
        return;
      }
    }
    data_exist = true;
  }

  if (((*message_)[strings::msg_params].keyExists(strings::vr_commands))
      && ((*message_)[strings::msg_params]
                      [strings::vr_commands].length() > 0)) {

    if (!CheckCommandVRSynonym(app)) {
      SendResponse(false, mobile_apis::Result::DUPLICATE_NAME);
      return;
    }

    if (!CheckVRCommandsNames()) {
      SendResponse(false, mobile_apis::Result::INVALID_DATA);
      return;
    }
    data_exist = true;
  }

  if (!data_exist) {
    LOG4CXX_ERROR_EXT(logger_, "INVALID_DATA");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::menu_params)) {
    if (!CheckMenuName()) {
      LOG4CXX_ERROR_EXT(logger_, "MenuName is invalid");
      SendResponse(false, mobile_apis::Result::INVALID_DATA);
      return;
    }
    smart_objects::SmartObject msg_params = smart_objects::SmartObject(
        smart_objects::SmartType_Map);
    msg_params[strings::cmd_id] =
        (*message_)[strings::msg_params][strings::cmd_id];
    msg_params[strings::menu_params] =
        (*message_)[strings::msg_params][strings::menu_params];

    msg_params[strings::app_id] = app->app_id();

    if (    ((*message_)[strings::msg_params].keyExists(strings::cmd_icon))
        &&  ((*message_)[strings::msg_params][strings::cmd_icon].keyExists(strings::value))
        &&  (0 < (*message_)[strings::msg_params][strings::cmd_icon]
                                                 [strings::value].length())) {
      msg_params[strings::cmd_icon] =
          (*message_)[strings::msg_params][strings::cmd_icon];
    }

    is_ui_send_ = true;

    SendHMIRequest(hmi_apis::FunctionID::UI_AddCommand, &msg_params, true);
  }

  if ((*message_)[strings::msg_params].keyExists(strings::vr_commands)) {
    smart_objects::SmartObject msg_params = smart_objects::SmartObject(
        smart_objects::SmartType_Map);
    msg_params[strings::cmd_id] =
        (*message_)[strings::msg_params][strings::cmd_id];
    msg_params[strings::vr_commands] =
        (*message_)[strings::msg_params][strings::vr_commands];
    msg_params[strings::app_id] = app->app_id();

    is_vr_send_ = true;

    SendHMIRequest(hmi_apis::FunctionID::VR_AddCommand, &msg_params, true);
  }
}

bool AddCommandRequest::CheckCommandName(ApplicationConstSharedPtr app) {
  if (!app) {
    return false;
  }

  const CommandsMap& commands = app->commands_map();
  CommandsMap::const_iterator i = commands.begin();

  for (; commands.end() != i; ++i) {

    if(!(*i->second).keyExists(strings::menu_params)) {
      continue;
    }

    if ((*i->second)[strings::menu_params][strings::menu_name].asString()
        == (*message_)[strings::msg_params][strings::menu_params]
                                            [strings::menu_name].asString()) {
      LOG4CXX_INFO(logger_, "AddCommandRequest::CheckCommandName received"
                   " command name already exist");
      return false;
    }
  }
  return true;
}

bool AddCommandRequest::CheckCommandVRSynonym(ApplicationConstSharedPtr app) {
  if (!app) {
    return false;
  }

  const CommandsMap& commands = app->commands_map();
  CommandsMap::const_iterator it = commands.begin();

  for (; commands.end() != it; ++it) {

    if(!(*it->second).keyExists(strings::vr_commands)) {
      continue;
    }

    for (size_t i = 0; i < (*it->second)[strings::vr_commands].length(); ++i) {
      for (size_t j = 0;
          j < (*message_)[strings::msg_params][strings::vr_commands].length();
          ++j) {
        std::string vr_cmd_i =
            (*it->second)[strings::vr_commands][i].asString();
        std::string vr_cmd_j =
            (*message_)[strings::msg_params]
                        [strings::vr_commands][j].asString();

        if (0 == strcasecmp(vr_cmd_i.c_str(), vr_cmd_j.c_str())) {
          LOG4CXX_INFO(logger_, "AddCommandRequest::CheckCommandVRSynonym"
                       " received command vr synonym already exist");
          return false;
        }
      }
    }
  }
  return true;
}

bool AddCommandRequest::CheckVRCommandsNames() {
  for (size_t i = 0;
       i < (*message_)[strings::msg_params][strings::vr_commands].length();
       ++i) {
    const std::string& str =
        (*message_)[strings::msg_params][strings::vr_commands][i].asString();

    if (!CheckSyntax(str)) {
      LOG4CXX_INFO(logger_, "Invalid command name.");
      return false;
    }
  }
  return true;
}

bool AddCommandRequest::CheckMenuName() {
    const std::string& str = (*message_)[strings::msg_params][strings::menu_params]
                             [strings::menu_name].asString();
    if (!CheckSyntax(str)) {
      LOG4CXX_INFO(logger_, "Invalid menu name.");
      return false;
    }
  return true;
}

bool AddCommandRequest::CheckCommandParentId(ApplicationConstSharedPtr app) {
  if (!app) {
    return false;
  }

  const int32_t parent_id =
      (*message_)[strings::msg_params][strings::menu_params]
                                       [hmi_request::parent_id].asInt();
  smart_objects::SmartObject* parent = app->FindSubMenu(parent_id);

  if (!parent) {
    LOG4CXX_INFO(logger_, "AddCommandRequest::CheckCommandParentId received"
                 " submenu doesn't exist");
    return false;
  }
  return true;
}

void AddCommandRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_INFO(logger_, "AddCommandRequest::on_event");
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::UI_AddCommand: {
      LOG4CXX_INFO(logger_, "Received UI_AddCommand event");
      is_ui_received_ = true;
      ui_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());

      if (hmi_apis::Common_Result::SUCCESS != ui_result_) {
           (*message_)[strings::msg_params].erase(strings::menu_params);
         }
      break;
    }
    case hmi_apis::FunctionID::VR_AddCommand: {
      LOG4CXX_INFO(logger_, "Received VR_AddCommand event");
      is_vr_received_ = true;
      vr_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());

      if (hmi_apis::Common_Result::SUCCESS != vr_result_) {
        (*message_)[strings::msg_params].erase(strings::vr_commands);
      }
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_,"Received unknown event" << event.id());
      return;
    }
  }

  if (!IsPendingResponseExist()) {
    ApplicationSharedPtr application =
        ApplicationManagerImpl::instance()->application(connection_key());

    if (!application) {
      LOG4CXX_ERROR(logger_, "NULL pointer");
      return;
    }

    smart_objects::SmartObject* command = application->FindCommand(
        (*message_)[strings::msg_params][strings::cmd_id].asInt());

    if (!command) {
      if ((((*message_)[strings::msg_params].keyExists(strings::menu_params)) ||
          ((*message_)[strings::msg_params].keyExists(strings::vr_commands))) &&
           (hmi_apis::Common_Result::REJECTED != ui_result_)) {
        application->AddCommand((*message_)[strings::msg_params]
                                      [strings::cmd_id].asInt(),
                                      (*message_)[strings::msg_params]);
      }

      mobile_apis::Result::eType result_code = mobile_apis::Result::INVALID_ENUM;

      bool result = ((hmi_apis::Common_Result::SUCCESS == ui_result_) &&
                     (hmi_apis::Common_Result::SUCCESS == vr_result_)) ||
                     ((hmi_apis::Common_Result::SUCCESS == ui_result_) &&
                     (hmi_apis::Common_Result::INVALID_ENUM == vr_result_)) ||
                     ((hmi_apis::Common_Result::INVALID_ENUM == ui_result_) &&
                     (hmi_apis::Common_Result::SUCCESS == vr_result_));

      if (!result && (hmi_apis::Common_Result::REJECTED == ui_result_)) {
        result_code = static_cast<mobile_apis::Result::eType>(ui_result_);
      } else {
        result_code = static_cast<mobile_apis::Result::eType>(
            std::max(ui_result_, vr_result_));
      }

      ApplicationSharedPtr application =
          ApplicationManagerImpl::instance()->application(connection_key());
      SendResponse(result, result_code, NULL, &(message[strings::msg_params]));
      application->UpdateHash();
    }
  }
}

bool AddCommandRequest::IsPendingResponseExist() {
  return is_ui_send_ != is_ui_received_ || is_vr_send_ != is_vr_received_;
}

}  // namespace commands

}  // namespace application_manager
