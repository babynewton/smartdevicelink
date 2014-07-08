/**
 * Copyright (c) 2014, Ford Motor Company
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

#include "application_manager/usage_statistics.h"

#include "smart_objects/enum_schema_item.h"
#include "usage_statistics/statistics_manager.h"
#include "utils/macro.h"

using namespace mobile_apis;
using namespace NsSmartDeviceLink::NsSmartObjects;
using namespace usage_statistics;

namespace application_manager {

namespace {

std::string LanguageIdToString(Language::eType lang_id) {
  typedef std::map<Language::eType, std::string> EnumMap;
  const EnumMap& enum_map =
    TEnumSchemaItem<Language::eType>::getEnumElementsStringRepresentation();
  EnumMap::const_iterator found = enum_map.find(lang_id);
  if (found != enum_map.end()) {
    return found->second;
  } else {
    return "unknown";
  }
}

}  // namespace

ApplicationUsageReport::ApplicationUsageReport(
    const std::string& app_id,
    usage_statistics::StatisticsManager* statistics_manager)
  : time_in_hmi_state_(statistics_manager, app_id),
    app_registration_language_gui_(statistics_manager, app_id,
                                   LANGUAGE_GUI),
    app_registration_language_vui_(statistics_manager, app_id,
                                   LANGUAGE_VUI),
    count_of_rejected_rpc_calls_(statistics_manager, app_id,
                              REJECTED_RPC_CALLS),
    count_of_rpcs_sent_in_hmi_none_(statistics_manager, app_id,
                              RPCS_IN_HMI_NONE) {
  time_in_hmi_state_.Start(SECONDS_HMI_NONE);
}

void ApplicationUsageReport::RecordHmiStateChanged(
    mobile_apis::HMILevel::eType new_hmi_level) {
  using namespace mobile_apis::HMILevel;
  AppStopwatchId next_stopwatch = SECONDS_HMI_NONE;
  switch(new_hmi_level) {
    case HMI_FULL:
      next_stopwatch = SECONDS_HMI_FULL;
      break;
    case HMI_LIMITED:
      next_stopwatch = SECONDS_HMI_LIMITED;
      break;
    case HMI_BACKGROUND:
      next_stopwatch = SECONDS_HMI_BACKGROUND;
      break;
    case HMI_NONE:
      next_stopwatch = SECONDS_HMI_NONE;
      break;
    default:
      NOTREACHED();
  }
  time_in_hmi_state_.Switch(next_stopwatch);
}

void ApplicationUsageReport::RecordAppRegistrationGuiLanguage(
    Language::eType gui_language) {
  app_registration_language_gui_.Update(LanguageIdToString(gui_language));
}

void ApplicationUsageReport::RecordAppRegistrationVuiLanguage(
    mobile_apis::Language::eType vui_language) {
  app_registration_language_gui_.Update(LanguageIdToString(vui_language));
}

void ApplicationUsageReport::RecordRpcSentInHMINone() {
  ++count_of_rpcs_sent_in_hmi_none_;
}

void ApplicationUsageReport::RecordPolicyRejectedRpcCall() {
  ++count_of_rejected_rpc_calls_;
}

}  // namespace application_manager
