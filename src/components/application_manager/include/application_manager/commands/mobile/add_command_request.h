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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_ADD_COMMAND_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_ADD_COMMAND_REQUEST_H_

#include "application_manager/application.h"
#include "application_manager/commands/command_request_impl.h"
#include "utils/macro.h"

namespace application_manager {

namespace commands {

/**
 * @brief AddCommandRequest command class
 **/
class AddCommandRequest : public CommandRequestImpl {
 public:
  /**
   * @brief AddCommandRequest class constructor
   *
   * @param message Incoming SmartObject message
   **/
  explicit AddCommandRequest(const MessageSharedPtr& message);

  /**
   * @brief AddCommandRequest class destructor
   **/
  virtual ~AddCommandRequest();

  /**
   * @brief Execute command
   **/
  virtual void Run();

  /**
   * @brief Interface method that is called whenever new event received
   *
   * @param event The received event
   */
  void on_event(const event_engine::Event& event);

 private:

  /*
   * @brief Check if command name doesn't exist in application
   * Please see SDLAQ-CRS-407 for more information
   *
   * @param app Mobile application
   *
   * @return TRUE on success, otherwise FALSE
   */
  bool CheckCommandName(ApplicationConstSharedPtr app);

  /*
   * @brief Check if command VR synonyms doesn't exist in application commands
   * Please see SDLAQ-CRS-407 for more information
   *
   * @param app Mobile application
   *
   * @return TRUE on success, otherwise FALSE
   */
  bool CheckCommandVRSynonym(ApplicationConstSharedPtr app);

  /*
   * @brief Check if command parent ID exists in submenu map
   *
   * @param app Mobile application
   *
   * @return TRUE on success, otherwise FALSE
   */
  bool CheckCommandParentId(ApplicationConstSharedPtr app);

  /*
   * @brief Check if commands has valid syntax
   *
   * @return TRUE on success, otherwise FALSE
   */
  bool CheckVRCommandsNames();

    /*
   * @brief Check if menuNames has valid syntax
   *
   * @return TRUE on success, otherwise FALSE
   */
  bool CheckMenuName();

  DISALLOW_COPY_AND_ASSIGN(AddCommandRequest);

  /*
   * @brief Check if there some not delivered hmi responses exist
   *
   * @return true if all responses received
   */
  bool IsPendingResponseExist();

  bool is_ui_send_;
  bool is_vr_send_;

  bool is_ui_received_;
  bool is_vr_received_;

  hmi_apis::Common_Result::eType ui_result_;
  hmi_apis::Common_Result::eType vr_result_;
};

}  // namespace commands
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_ADD_COMMAND_REQUEST_H_
