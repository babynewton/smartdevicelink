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
#include <algorithm>
#include <utility>
#include "policy/sql_pt_ext_representation.h"
#include "policy/sql_wrapper.h"
#include "policy/sql_pt_queries.h"
#include "policy/sql_pt_ext_queries.h"

namespace policy {

bool SQLPTExtRepresentation::CanAppKeepContext(const std::string& app_id) {
  dbms::SQLQuery query(db());
  if (query.Prepare(sql_pt_ext::kSelectKeepContext)) {
    query.Bind(0, app_id);
    if (query.Exec()) {
      return query.GetBoolean(0);
    }
  }
  return false;
}

bool SQLPTExtRepresentation::CanAppStealFocus(const std::string& app_id) {
  dbms::SQLQuery query(db());
  if (query.Prepare(sql_pt_ext::kSelectStealFocus)) {
    query.Bind(0, app_id);
    if (query.Exec()) {
      return query.GetBoolean(0);
    }
  }
  return false;
}

bool SQLPTExtRepresentation::ResetUserConsent() {
  dbms::SQLQuery query(db());
  if (query.Prepare(sql_pt_ext::kResetUserConsent)) {
    if (query.Exec()) {
      return query.GetBoolean(0);
    }
  }
  return false;
}

bool SQLPTExtRepresentation::GetUserPermissionsForDevice(
  const std::string& device_id, StringArray* consented_groups,
  StringArray* disallowed_groups) {
  LOG4CXX_INFO(logger(), "GetUserPermissionsForDevice");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kSelectDeviceConsentedGroup)) {
    LOG4CXX_WARN(logger(), "Incorrect select from device consented groups");
    return false;
  }
  query.Bind(0, device_id);
  while (query.Next()) {
    if (query.GetBoolean(2)) {
      if (!consented_groups) {
        continue;
      }
      consented_groups->push_back(query.GetString(1));
    } else {
      if (!disallowed_groups) {
        continue;
      }
      disallowed_groups->push_back(query.GetString(1));
    }
  }

  return true;
}

bool SQLPTExtRepresentation::GetUserPermissionsForApp(
  const std::string& device_id, const std::string& policy_app_id,
  std::vector<FunctionalGroupPermission>& permissions) {
  LOG4CXX_INFO(logger(), "GetUserPermissionsForApp");
  // Get all app groups for specified device and application
  FunctionalGroupIDs all_groups;
  if (!GetAllAppGroups(policy_app_id, all_groups)) {
    return false;
  }
  // Get preconsented group
  FunctionalGroupIDs preconsented_groups;
  if (!GetPreconsentedGroups(policy_app_id, preconsented_groups)) {
    return false;
  }
  // Get consented (allowed/disallowed) groups
  FunctionalGroupIDs allowed_groups;
  FunctionalGroupIDs disallowed_groups;
  if (!GetConsentedGroups(policy_app_id, device_id,
                          allowed_groups, disallowed_groups)) {
    return false;
  }
  // Get group names
  FunctionalGroupNames names;
  if (!GetFunctionalGroupNames(names)) {
    return false;
  }

  std::sort(all_groups.begin(), all_groups.end());
  std::sort(allowed_groups.begin(), allowed_groups.end());
  std::sort(disallowed_groups.begin(), disallowed_groups.end());
  std::sort(preconsented_groups.begin(), preconsented_groups.end());

  // Find groups with undefinded consent
  FunctionalGroupIDs no_preconsented;
  std::set_difference(all_groups.begin(), all_groups.end(),
                      preconsented_groups.begin(), preconsented_groups.end(),
                      std::back_inserter(no_preconsented));
  FunctionalGroupIDs no_allowed_preconsented;
  std::set_difference(no_preconsented.begin(), no_preconsented.end(),
                      allowed_groups.begin(), allowed_groups.end(),
                      std::back_inserter(no_allowed_preconsented));
  FunctionalGroupIDs undefined_consent;
  std::set_difference(no_allowed_preconsented.begin(), no_allowed_preconsented.end(),
                      disallowed_groups.begin(), disallowed_groups.end(),
                      std::back_inserter(undefined_consent));

  // Find common allowed groups
  FunctionalGroupIDs preconsented_allowed;
  std::merge(allowed_groups.begin(), allowed_groups.end(),
             preconsented_groups.begin(), preconsented_groups.end(),
             std::back_inserter(preconsented_allowed));
  FunctionalGroupIDs common_allowed(preconsented_allowed.begin(),
                                    std::unique(preconsented_allowed.begin(), preconsented_allowed.end()));

  // Find common disallowed groups
  FunctionalGroupIDs common_disallowed;
  std::set_difference(disallowed_groups.begin(), disallowed_groups.end(),
                      preconsented_groups.begin(), preconsented_groups.end(),
                      std::back_inserter(common_disallowed));

  // Fill result
  FillFunctionalGroupPermissions(undefined_consent, names, kUndefined, permissions);
  FillFunctionalGroupPermissions(common_allowed, names, kAllowed, permissions);
  FillFunctionalGroupPermissions(common_disallowed, names, kDisallowed, permissions);

  return true;
}

bool SQLPTExtRepresentation::GetDeviceGroupsFromPolicies(
  policy_table::Strings* groups, policy_table::Strings* preconsented_groups) {
  LOG4CXX_INFO(logger(), "GetDeviceGroupsFromPolicies");
  if (groups) {
    GatherAppGroup("device", groups);
  }
  if (preconsented_groups) {
    GatherPreconsentedGroup("device", preconsented_groups);
  }
  return true;
}

bool SQLPTExtRepresentation::SetDeviceData(const std::string& device_id,
    const std::string& hardware,
    const std::string& firmware,
    const std::string& os,
    const std::string& os_version,
    const std::string& carrier,
    const uint32_t number_of_ports) {
  LOG4CXX_INFO(logger(), "SetDeviceData");
  dbms::SQLQuery count_query(db());
  if (!count_query.Prepare(sql_pt_ext::kCountDevice)) {
    LOG4CXX_WARN(logger(), "Incorrect statement for count of device.");
    return false;
  }

  count_query.Bind(0, device_id);

  if (!count_query.Exec()) {
    LOG4CXX_WARN(logger(), "Incorrect count of device.");
    return false;
  }

  bool update = count_query.GetInteger(0);

  // Update old value
  if (update) {
    dbms::SQLQuery update_query(db());
    if (!update_query.Prepare(sql_pt_ext::kUpdateDevice)) {
      LOG4CXX_WARN(logger(), "Incorrect statement for udpate device.");
      return false;
    }

    update_query.Bind(0, hardware);
    update_query.Bind(1, firmware);
    update_query.Bind(2, os);
    update_query.Bind(3, os_version);
    update_query.Bind(4, carrier);
    update_query.Bind(5, static_cast<int>(number_of_ports));
    update_query.Bind(6, device_id);

    if (!update_query.Exec() || !update_query.Reset()) {
      LOG4CXX_WARN(logger(), "Incorrect update for device.");
      return false;
    }

    return true;
  }

  // Insert new data
  dbms::SQLQuery insert_query(db());
  if (!insert_query.Prepare(sql_pt_ext::kInsertDevice)) {
    LOG4CXX_WARN(logger(), "Incorrect insert statement for device.");
    return false;
  }

  insert_query.Bind(0, device_id);
  insert_query.Bind(1, hardware);
  insert_query.Bind(2, firmware);
  insert_query.Bind(3, os);
  insert_query.Bind(4, os_version);
  insert_query.Bind(5, carrier);
  insert_query.Bind(6, static_cast<int>(number_of_ports));

  if (!insert_query.Exec() || !insert_query.Reset()) {
    LOG4CXX_WARN(logger(), "Incorrect insert to device.");
    return false;
  }

  return true;
}

bool SQLPTExtRepresentation::SetUserPermissionsForDevice(
  const std::string& device_id, const StringArray& consented_groups,
  const StringArray& disallowed_groups) {
  LOG4CXX_INFO(logger(), "SetUserPermissionsForDevice");
  dbms::SQLQuery count_query(db());
  if (!count_query.Prepare(sql_pt_ext::kCountDeviceConsentGroup)) {
    LOG4CXX_WARN(logger(), "Incorrect count of device consented groups");
    return false;
  }

  count_query.Bind(0, device_id);

  if (!count_query.Exec()) {
    LOG4CXX_WARN(logger(), "Incorrect count of device consented groups");
    return false;
  }

  bool update = count_query.GetInteger(0);

  // TODO(AOleynik): Split to several methods?
  dbms::SQLQuery query(db());
  // Update old values
  if (update) {
    if (!query.Prepare(sql_pt_ext::kUpdateDeviceConsentedGroup)) {
      LOG4CXX_WARN(logger(), "Incorrect update of device consented groups");
      return false;
    }

    StringArray::const_iterator it_consented_groups = consented_groups.begin();
    StringArray::const_iterator it_consented_groups_end =
      consented_groups.end();
    for (; it_consented_groups != it_consented_groups_end;
         ++it_consented_groups) {
      query.Bind(0, 1);
      query.Bind(1, device_id);
      query.Bind(2, *it_consented_groups);
      // TODO(AOleynik): Get this info from external data
      query.Bind(3, "GUI");
      if (!query.Exec() || !query.Reset()) {
        LOG4CXX_WARN(logger(), "Incorrect update of device consented groups.");
        return false;
      }
    }

    StringArray::const_iterator it_disallowed_groups =
      disallowed_groups.begin();
    StringArray::const_iterator it_disallowed_groups_end =
      disallowed_groups.end();
    for (; it_disallowed_groups != it_disallowed_groups_end;
         ++it_disallowed_groups) {
      query.Bind(0, 0);
      query.Bind(1, device_id);
      query.Bind(2, *it_disallowed_groups);
      if (!query.Exec() || !query.Reset()) {
        LOG4CXX_WARN(logger(), "Incorrect update of device consented groups.");
        return false;
      }
    }

    return true;
  }

  // Insert new values
  if (!query.Prepare(sql_pt_ext::kInsertDeviceConsentedGroup)) {
    LOG4CXX_WARN(logger(), "Incorrect insert to device consented groups");
    return false;
  }

  StringArray::const_iterator it_consented_groups = consented_groups.begin();
  StringArray::const_iterator it_consented_groups_end = consented_groups.end();
  for (; it_consented_groups != it_consented_groups_end;
       ++it_consented_groups) {
    query.Bind(0, device_id);
    query.Bind(1, *it_consented_groups);
    query.Bind(2, 1);
    // TODO(AOleynik): Get this info from external data
    query.Bind(3, std::string("GUI"));
    if (!query.Exec() || !query.Reset()) {
      LOG4CXX_WARN(logger(), "Incorrect insert to device consented groups.");
      return false;
    }
  }

  StringArray::const_iterator it_disallowed_groups = disallowed_groups.begin();
  StringArray::const_iterator it_disallowed_groups_end =
    disallowed_groups.end();
  for (; it_disallowed_groups != it_disallowed_groups_end;
       ++it_disallowed_groups) {
    query.Bind(0, device_id);
    query.Bind(1, *it_disallowed_groups);
    query.Bind(2, 0);
    if (!query.Exec() || !query.Reset()) {
      LOG4CXX_WARN(logger(), "Incorrect insert to device consented groups.");
      return false;
    }
  }

  return true;
}

bool SQLPTExtRepresentation::SetUserPermissionsForApp(
  const PermissionConsent& permissions) {
  LOG4CXX_INFO(logger(), "SetUserPermissionsForApp");
  // TODO(AOleynik): Handle situation, when no application was specified, i.e.
  // general permissions were set
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kUpdateGroupPermissions)) {
    LOG4CXX_WARN(logger(),
                 "Incorrect statement for update app group permissions.");
    return false;
  }

  std::vector<FunctionalGroupPermission>::const_iterator it = permissions
      .group_permissions.begin();
  std::vector<FunctionalGroupPermission>::const_iterator it_end = permissions
      .group_permissions.end();
  for (; it != it_end; ++it) {
    // Skip consent saving, if user didn't choose any state
    if (policy::kUndefined == (*it).state) {
      continue;
    }
    query.Bind(0, (*it).state);
    query.Bind(1, permissions.consent_source);
    query.Bind(2, permissions.policy_app_id);
    query.Bind(3, static_cast<int>((*it).group_id));
    query.Bind(4, permissions.device_id);

    if (!query.Exec() || !query.Reset()) {
      LOG4CXX_WARN(logger(), "Incorrect update on user defined permissions "
                   "for app groups.");
      return false;
    }
  }

  return true;
}

std::vector<UserFriendlyMessage> SQLPTExtRepresentation::GetUserFriendlyMsg(
  const std::vector<std::string>& msg_codes, const std::string& language) {
  dbms::SQLQuery query(db());
  std::vector<UserFriendlyMessage> result;
  if (!query.Prepare(sql_pt_ext::kSelectFriendlyMsg)) {
    LOG4CXX_WARN(logger(), "Incorrect statement for select friendly messages.");
    return result;
  }

  std::vector<std::string>::const_iterator it = msg_codes.begin();
  std::vector<std::string>::const_iterator it_end = msg_codes.end();
  for (; it != it_end; ++it) {
    query.Bind(0, *it);
    query.Bind(1, language);

    if (!query.Exec()) {
      LOG4CXX_WARN(logger(), "Incorrect select from friendly messages.");
      return result;
    }

    UserFriendlyMessage msg;

    msg.tts = query.GetString(0);
    msg.label = query.GetString(1);
    msg.line1 = query.GetString(2);
    msg.line2 = query.GetString(3);
    msg.text_body = query.GetString(4);

    result.push_back(msg);

    if (!query.Reset()) {
      LOG4CXX_WARN(logger(), "Faild reset statement for selecting friendly messages.");
      return result;
    }
  }

  return result;
}

bool SQLPTExtRepresentation::SetMetaInfo(const std::string& ccpu_version,
                                         const std::string& wers_country_code,
                                         const std::string& language) {
  LOG4CXX_INFO(logger(), "SetMetaInfo");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kUpdateMetaParams)) {
    LOG4CXX_WARN(logger(), "Incorrect statement for insert to module meta.");
    return false;
  }

  query.Bind(0, ccpu_version);
  query.Bind(1, wers_country_code);
  query.Bind(2, language);

  if (!query.Exec() || !query.Reset()) {
    LOG4CXX_WARN(logger(), "Incorrect insert to module meta.");
    return false;
  }

  return true;
}

bool SQLPTExtRepresentation::SetSystemLanguage(const std::string& language) {
  LOG4CXX_INFO(logger(), "SetSystemLanguage");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kUpdateMetaLanguage)) {
    LOG4CXX_WARN(logger(), "Incorrect statement for update meta language.");
    return false;
  }

  query.Bind(0, language);

  if (!query.Exec()) {
    LOG4CXX_WARN(logger(), "Incorrect update for meta language.");
    return false;
  }

  return true;
}

bool SQLPTExtRepresentation::SaveApplicationPolicies(
  const policy_table::ApplicationPolicies& apps) {
  LOG4CXX_INFO(logger(), "SaveApplicationPolicies ext");
  dbms::SQLQuery query_delete(db());
  if (!query_delete.Exec(sql_pt::kDeleteAppGroup)) {
    LOG4CXX_WARN(logger(), "Incorrect delete from app_group.");
    return false;
  }

  dbms::SQLQuery query_delete_preconsented(db());
  if (!query_delete_preconsented.Exec(sql_pt_ext::kDeletePreconsentedGroups)) {
    LOG4CXX_WARN(logger(), "Incorrect delete from preconsented_group.");
    return false;
  }

  policy_table::ApplicationPolicies::const_iterator it;
  dbms::SQLQuery app_query(db());
  if (!app_query.Prepare(sql_pt_ext::kInsertApplication)) {
    LOG4CXX_WARN(logger(), "Incorrect insert statement into application.");
    return false;
  }
  for (it = apps.begin(); it != apps.end(); ++it) {
    app_query.Bind(0, it->first);
    app_query.Bind(1, it->second.keep_context);
    app_query.Bind(2, it->second.steal_focus);
    app_query.Bind(
      3, std::string(policy_table::EnumToJsonString(it->second.default_hmi)));
    app_query.Bind(
      4, std::string(policy_table::EnumToJsonString(it->second.priority)));
    app_query.Bind(
      5, it->second.is_null());
    app_query.Bind(6, it->second.memory_kb);
    app_query.Bind(7, it->second.watchdog_timer_ms);
    it->second.certificate.is_initialized() ?
        app_query.Bind(8, it->second.certificate) : app_query.Bind(8,std::string());

    if (!app_query.Exec() || !app_query.Reset()) {
      LOG4CXX_WARN(logger(), "Incorrect insert into application.");
      return false;
    }

    LOG4CXX_INFO(logger(), "Saving data for application: " << it->first);
    if (!SaveAppGroup(it->first, it->second.groups)) {
      return false;
    }
    if (!SaveNickname(it->first, it->second.nicknames)) {
      return false;
    }
    if (!SaveAppType(it->first, it->second.AppHMIType)) {
      return false;
    }
    if (!SavePreconsentedGroup(it->first, it->second.preconsented_groups)) {
      return false;
    }
  }

  return true;
}

bool SQLPTExtRepresentation::GatherApplicationPolicies(
  policy_table::ApplicationPolicies* apps) const {
  LOG4CXX_INFO(logger(), "Gather applications policies");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kSelectAppPolicies)) {
    LOG4CXX_WARN(logger(), "Incorrect select from app_policies");
    return false;
  }

  while (query.Next()) {
    policy_table::ApplicationParams params;
    const std::string& app_id = query.GetString(0);
    policy_table::Priority priority;
    policy_table::EnumFromJsonString(query.GetString(1), &priority);
    params.priority = priority;
    policy_table::HmiLevel hmi;
    policy_table::EnumFromJsonString(query.GetString(2), &hmi);
    params.default_hmi = hmi;
    params.keep_context = query.GetBoolean(3);
    params.steal_focus = query.GetBoolean(4);
    params.memory_kb = query.GetInteger(5);
    params.watchdog_timer_ms = query.GetInteger(6);
    if (!query.IsNull(7)) {
      *params.certificate = query.GetString(7);
    }
    if (!GatherAppGroup(app_id, &params.groups)) {
      return false;
    }
    if (!GatherNickName(app_id, &params.nicknames)) {
      return false;
    }
    if (!GatherAppType(app_id, &params.AppHMIType)) {
      return false;
    }
    GatherPreconsentedGroup(app_id, &params.preconsented_groups);
    (*apps)[app_id] = params;
  }
  return true;
}

void SQLPTExtRepresentation::GatherPreconsentedGroup(
  const std::string& app_id, policy_table::Strings* groups) const {
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kSelectPreconsentedGroups)) {
    LOG4CXX_WARN(logger(), "Incorrect select from preconsented group");
    return;
  }

  query.Bind(0, app_id);
  while (query.Next()) {
    groups->push_back(query.GetString(0));
  }
}

bool SQLPTExtRepresentation::GatherUsageAndErrorCounts(
  policy_table::UsageAndErrorCounts* counts) const {
  LOG4CXX_INFO(logger(), "Gather Usage and Error Counts.");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kSelectUsageAndErrorCount) || !query.Exec()) {
    LOG4CXX_INFO(logger(), "Failed select from user_and_error_count");
    return false;
  }

  *counts->count_of_iap_buffer_full = query.GetInteger(0);
  *counts->count_sync_out_of_memory = query.GetInteger(1);
  *counts->count_of_sync_reboots = query.GetInteger(2);

  return GatherAppLevels(&counts->app_level);
}

bool SQLPTExtRepresentation::GatherAppLevels(
  policy_table::AppLevels* apps) const {
  dbms::SQLQuery query(db());
  if (query.Prepare(sql_pt_ext::kSelectAppLevels)) {
    LOG4CXX_INFO(logger(), "Failed select from app_level");
    return false;
  }
  const int kSecondsInMinute = 60;
  while (query.Next()) {
    policy_table::AppLevel level;
    // value of time fields database is seconds
    level.minutes_in_hmi_full = query.GetInteger(1) / kSecondsInMinute;
    level.minutes_in_hmi_limited = query.GetInteger(2) / kSecondsInMinute;
    level.minutes_in_hmi_background = query.GetInteger(3) / kSecondsInMinute;
    level.minutes_in_hmi_none = query.GetInteger(4) / kSecondsInMinute;
    level.count_of_rfcom_limit_reached = query.GetInteger(5);
    level.count_of_user_selections = query.GetInteger(6);
    level.count_of_rejections_sync_out_of_memory = query.GetInteger(7);
    level.count_of_rejections_nickname_mismatch = query.GetInteger(8);
    level.count_of_rejections_duplicate_name = query.GetInteger(9);
    level.count_of_rejected_rpc_calls = query.GetInteger(10);
    level.count_of_rpcs_sent_in_hmi_none = query.GetInteger(11);
    level.count_of_removals_for_bad_behavior = query.GetInteger(12);
    level.count_of_run_attempts_while_revoked = query.GetInteger(13);
    level.app_registration_language_gui = query.GetString(14);
    level.app_registration_language_vui = query.GetString(15);
    (*apps)[query.GetString(0)] = level;
  }

  return true;
}

void SQLPTExtRepresentation::GatherDeviceData(
  policy_table::DeviceData* data) const {
  LOG4CXX_INFO(logger(), "Gather device data.");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kSelectDeviceData)) {
    LOG4CXX_WARN(logger(), "Incorrect select statement for device data.");
    return;
  }

  while (query.Next()) {
    policy_table::DeviceParams* specific_device = &(*data)[query.GetString(0)];
    *specific_device->hardware = query.GetString(1);
    *specific_device->firmware_rev = query.GetString(2);
    *specific_device->os = query.GetString(3);
    *specific_device->os_version = query.GetString(4);
    *specific_device->carrier = query.GetString(5);
    *specific_device->max_number_rfcom_ports = query.GetInteger(6);

    GatherConsentGroup(query.GetString(0),
                       &(specific_device->user_consent_records));
  }
}

void SQLPTExtRepresentation::GatherConsentGroup(
  const std::string& device_id,
  policy_table::UserConsentRecords* records) const {
  LOG4CXX_INFO(logger(), "Gather consent records.");
  dbms::SQLQuery query(db());
  // Fill data for device
  if (!query.Prepare(sql_pt_ext::kSelectDeviceConsentedGroup)) {
    LOG4CXX_WARN(logger(),
                 "Incorrect select statement for device consented groups.");
    return;
  }

  query.Bind(0, device_id);

  // Fill device_data -> user_consent_records -> "device"
  policy_table::ConsentRecords* device_consent_records = &(*records)["device"];

  while (query.Next()) {
    (*device_consent_records).consent_groups[query.GetString(1)] = query
        .GetBoolean(2);
    policy_table::Input input;
    policy_table::EnumFromJsonString(query.GetString(3), &input);
    *device_consent_records->input = input;
    *device_consent_records->time_stamp = query.GetString(4);
  }

  if (!query.Reset()) {
    return;
  }

  // Fill data for applications
  if (!query.Prepare(sql_pt_ext::kSelectConsentGroup)) {
    LOG4CXX_WARN(logger(),
                 "Incorrect select statement for app consented groups.");
    return;
  }

  query.Bind(0, device_id);

  // Fill device_data -> user_consent_records -> <app_id>
  while (query.Next()) {
    policy_table::ConsentRecords* app_consent_records = &(*records)[query
        .GetString(1)];
    (*app_consent_records).consent_groups[query.GetString(2)] =
      query.GetBoolean(3);
    policy_table::Input input;
    policy_table::EnumFromJsonString(query.GetString(4), &input);
    *app_consent_records->input = input;
    *app_consent_records->time_stamp = query.GetString(5);
  }
}

bool SQLPTExtRepresentation::SaveDeviceData(
  const policy_table::DeviceData& devices) {
  LOG4CXX_INFO(logger(), "SaveDeviceData");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kInsertDeviceData)) {
    LOG4CXX_WARN(logger(), "Incorrect insert statement for device data.");
    return false;
  }

  policy_table::DeviceData::const_iterator it = devices.begin();
  policy_table::DeviceData::const_iterator it_end = devices.end();
  for (; it != it_end; ++it) {
    query.Bind(0, it->first);
    query.Bind(1, *(it->second.hardware));
    query.Bind(2, *(it->second.firmware_rev));
    query.Bind(3, *(it->second.os));
    query.Bind(4, *(it->second.os_version));
    query.Bind(5, *(it->second.carrier));
    query.Bind(6, *(it->second.max_number_rfcom_ports));

    if (!query.Exec() || !query.Reset()) {
      LOG4CXX_WARN(logger(), "Incorrect insert into device data.");
      return false;
    }

    if (!SaveConsentGroup(it->first, it->second.user_consent_records)) {
      return false;
    }
  }

  return true;
}

bool SQLPTExtRepresentation::SaveConsentGroup(
  const std::string& device_id,
  const policy_table::UserConsentRecords& records) {
  LOG4CXX_INFO(logger(), "SaveConsentGroup");
  dbms::SQLQuery query(db());

  policy_table::UserConsentRecords::const_iterator it = records.begin();
  policy_table::UserConsentRecords::const_iterator it_end = records.end();
  for (; it != it_end; ++it) {
    policy_table::ConsentGroups::const_iterator it_groups = it->second
        .consent_groups.begin();
    policy_table::ConsentGroups::const_iterator it_groups_end = it->second
        .consent_groups.end();
    for (; it_groups != it_groups_end; ++it_groups) {
      if ("device" == it->first) {
        if (!query.Prepare(sql_pt_ext::kInsertDeviceConsentedGroup)) {
          LOG4CXX_WARN(logger(),
                       "Incorrect insert statement for device consent group.");
          return false;
        }
        query.Bind(0, device_id);
        query.Bind(1, it_groups->first);
        query.Bind(2, it_groups->second);
        query.Bind(
          3,
          std::string(policy_table::EnumToJsonString(*(it->second.input))));
      } else {
        if (!query.Prepare(sql_pt_ext::kInsertConsentGroups)) {
          LOG4CXX_WARN(logger(),
                       "Incorrect insert statement for consent group.");
          return false;
        }
        query.Bind(0, device_id);
        query.Bind(1, it->first);
        query.Bind(2, it_groups->first);
        query.Bind(3, it_groups->second);
        query.Bind(
          4,
          std::string(policy_table::EnumToJsonString(*(it->second.input))));
      }

      if (!query.Exec() && !query.Reset()) {
        LOG4CXX_WARN(logger(), "Incorrect insert into consent group.");
        return false;
      }
    }
  }

  return true;
}

bool SQLPTExtRepresentation::SavePreconsentedGroup(
  const std::string& app_id, const policy_table::Strings& groups) {
  LOG4CXX_INFO(logger(), "SavePreconsentedGroup");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kInsertPreconsentedGroups)) {
    LOG4CXX_WARN(logger(),
                 "Incorrect insert statement for preconsented groups");
    return false;
  }

  policy_table::Strings::const_iterator it;
  for (it = groups.begin(); it != groups.end(); ++it) {
    query.Bind(0, app_id);
    query.Bind(1, *it);
    if (!query.Exec() || !query.Reset()) {
      LOG4CXX_WARN(logger(), "Incorrect insert into preconsented groups.");
      return false;
    }
  }

  return true;
}

void SQLPTExtRepresentation::GatherModuleMeta(
  policy_table::ModuleMeta* meta) const {
  LOG4CXX_INFO(logger(), "Gather Module Meta Info");
  dbms::SQLQuery query(db());
  if (query.Prepare(sql_pt_ext::kSelectModuleMeta) && query.Next()) {
    *meta->ccpu_version = query.GetString(0);
    *meta->language = query.GetString(1);
    *meta->wers_country_code = query.GetString(2);
    *meta->pt_exchanged_at_odometer_x = query.GetInteger(3);
    *meta->pt_exchanged_x_days_after_epoch = query.GetInteger(4);
    *meta->ignition_cycles_since_last_exchange = query.GetInteger(5);
    *meta->vin = query.GetString(6);
  }
}

void SQLPTExtRepresentation::Increment(const std::string& type) const {
  dbms::SQLQuery query(db());
  std::string update_counter = "UPDATE `usage_and_error_count` SET `" + type
                               + "` = `" + type + "` + 1";
  if (!query.Exec(update_counter)) {
    LOG4CXX_INFO(logger(), "Failed updating global counter");
  }
}

bool SQLPTExtRepresentation::IsExistAppLevel(const std::string& app_id) const {
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kCountAppLevel)) {
    LOG4CXX_INFO(logger(), "Incorrect statement of count app_level");
    return false;
  }
  query.Bind(0, app_id);
  if (!query.Exec()) {
    LOG4CXX_INFO(logger(), "Failed count app_level");
    return false;
  }
  return query.GetInteger(0) > 0;
}

bool SQLPTExtRepresentation::GetAllAppGroups(const std::string& policy_app_id,
    FunctionalGroupIDs& all_groups) {
  LOG4CXX_INFO(logger(), "GetAllAppGroups");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kSelectAppGroupsId)) {
    LOG4CXX_WARN(logger(), "Incorrect statement for select app groups id.");
    return false;
  }

  query.Bind(0, policy_app_id);

  while (query.Next()) {
    all_groups.push_back(query.GetInteger(0));
  }

  return true;
}

bool SQLPTExtRepresentation::GetConsentedGroups(
  const std::string& policy_app_id, const std::string& device_id,
  FunctionalGroupIDs& allowed_groups, FunctionalGroupIDs& disallowed_groups) {

  LOG4CXX_INFO(logger(), "GetConsentedGroups");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kSelectConsentedGroupsId)) {
    LOG4CXX_WARN(logger(), "Incorrect statement for select consent groups id.");
    return false;
  }

  query.Bind(0, policy_app_id);
  query.Bind(1, device_id);

  while (query.Next()) {
    if (query.GetBoolean(1)) {
      allowed_groups.push_back(query.GetInteger(0));
    } else {
      disallowed_groups.push_back(query.GetInteger(0));
    }
  }

  return true;
}

bool SQLPTExtRepresentation::GetPreconsentedGroups(
  const std::string& policy_app_id, FunctionalGroupIDs& preconsented_groups) {
  LOG4CXX_INFO(logger(), "GetPreconsentedGroups");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kSelectPreconsentedGroupsId)) {
    LOG4CXX_WARN(logger(),
                 "Incorrect statement for select preconsented groups id.");
    return false;
  }

  query.Bind(0, policy_app_id);

  while (query.Next()) {
    preconsented_groups.push_back(query.GetInteger(0));
  }

  return true;
}

bool SQLPTExtRepresentation::GetFunctionalGroupNames(
  FunctionalGroupNames& names) {
  LOG4CXX_INFO(logger(), "GetFunctionalGroupNames");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kSelectFunctionalGroupNames)) {
    LOG4CXX_WARN(logger(),
                 "Incorrect statement for select functional groups names.");
    return false;
  }

  while (query.Next()) {
    // Some of functional grous doesn't have filled user_consent_prompt
    if (query.IsNull(1)) {
      names[query.GetInteger(0)] =
          std::make_pair<std::string, std::string>("", query.GetString(2));
    } else {
      names[query.GetInteger(0)] =
          std::make_pair<std::string, std::string>(query.GetString(1), query.GetString(2));
    }

  }

  return true;
}

void SQLPTExtRepresentation::FillFunctionalGroupPermissions(
  FunctionalGroupIDs& ids, FunctionalGroupNames& names, PermissionState state,
  std::vector<FunctionalGroupPermission>& permissions) {
  FunctionalGroupIDs::const_iterator it = ids.begin();
  FunctionalGroupIDs::const_iterator it_end = ids.end();
  for (; it != it_end; ++it) {
    FunctionalGroupPermission current_group;
    current_group.group_id = *it;
    current_group.group_alias = names[*it].first;
    current_group.group_name = names[*it].second;
    current_group.state = state;
    permissions.push_back(current_group);
  }
}

void SQLPTExtRepresentation::Increment(const std::string& app_id,
                                       const std::string& type) const {
  dbms::SQLQuery query(db());
  std::string sql_counter;
  if (IsExistAppLevel(app_id)) {
    // update
    sql_counter = "UPDATE `app_level` SET `" + type + "` = `" + type
                  + "` + 1 WHERE `application_id` = ?";
  } else {
    // insert
    sql_counter = "INSERT INTO `app_level` (`application_id`, `" + type + "`) "
                  "VALUES (?, 1)";
  }
  if (!query.Prepare(sql_counter)) {
    LOG4CXX_INFO(logger(), "Incorrect statement of update app counter");
    return;
  }
  query.Bind(0, app_id);
  if (!query.Exec()) {
    LOG4CXX_INFO(logger(), "Failed updating app counter");
  }
}

void SQLPTExtRepresentation::Set(const std::string& app_id,
                                 const std::string& type,
                                 const std::string& value) const {
  dbms::SQLQuery query(db());
  std::string sql_info;
  if (IsExistAppLevel(app_id)) {
    // update
    sql_info = "UPDATE `app_level` SET `" + type + "` = ? "
               "WHERE `application_id` = ?";
  } else {
    // insert
    sql_info = "INSERT INTO `app_level` (`" + type + "`, `application_id`) "
               "VALUES (?, ?)";
  }
  if (!query.Prepare(sql_info)) {
    LOG4CXX_INFO(logger(), "Incorrect statement of update app info");
    return;
  }
  query.Bind(0, value);
  query.Bind(1, app_id);
  if (!query.Exec()) {
    LOG4CXX_INFO(logger(), "Failed updating app info");
  }
}

void SQLPTExtRepresentation::Add(const std::string& app_id,
                                 const std::string& type, int seconds) const {
  dbms::SQLQuery query(db());
  std::string sql_stopwatch;
  if (IsExistAppLevel(app_id)) {
    // update
    sql_stopwatch = "UPDATE `app_level` SET `" + type + "` = `" + type
                    + "` + ? WHERE `application_id` = ?";
  } else {
    // insert
    sql_stopwatch = "INSERT INTO `app_level` (`" + type
                    + "`, `application_id`) "
                    "VALUES (?, ?)";
  }
  if (!query.Prepare(sql_stopwatch)) {
    LOG4CXX_INFO(logger(), "Incorrect statement of update app stopwatch");
    return;
  }
  query.Bind(0, seconds);
  query.Bind(1, app_id);
  if (!query.Exec()) {
    LOG4CXX_INFO(logger(), "Failed updating app stopwatch");
  }
}

bool SQLPTExtRepresentation::GetDefaultHMI(const std::string& policy_app_id,
    std::string* default_hmi) {
  LOG4CXX_INFO(logger(), "GetDefaultHMI");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kSelectDefaultHmi)) {
    LOG4CXX_INFO(logger(), "Incorrect statement for default hmi.");
    return false;
  }

  query.Bind(0, policy_app_id);

  if (!query.Exec() || !query.Reset()) {
    LOG4CXX_INFO(logger(), "Error during default hmi getting.");
    return false;
  }

  if (query.IsNull(0)) {
    default_hmi->clear();
    return true;
  }

  default_hmi->assign(query.GetString(0));

  return true;
}

bool SQLPTExtRepresentation::GetPriority(const std::string& policy_app_id,
    std::string* priority) {
  LOG4CXX_INFO(logger(), "GetPriority");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kSelectPriority)) {
    LOG4CXX_INFO(logger(), "Incorrect statement for priority.");
    return false;
  }

  query.Bind(0, policy_app_id);

  if (!query.Exec()) {
    LOG4CXX_INFO(logger(), "Error during select priority.");
    return false;
  }

  if (query.IsNull(0)) {
    priority->clear();
    return true;
  }

  priority->assign(query.GetString(0));

  return true;
}

bool SQLPTExtRepresentation::CountUnconsentedGroups(
  const std::string& policy_app_id, int* result) const {
  LOG4CXX_INFO(logger(), "CountUnconsentedGroups");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kCountUnconsentedGroups)) {
    LOG4CXX_WARN(logger(), "Incorrect select for unconsented groups.");
    return false;
  }

  query.Bind(0, policy_app_id);

  if (!query.Exec() || !query.Reset()) {
    LOG4CXX_INFO(logger(), "Error during executing unconsented groups.");
    return false;
  }
  *result = query.GetInteger(0);
  return true;

}

bool SQLPTExtRepresentation::SaveMessageString(
  const std::string& type, const std::string& lang,
  const policy_table::MessageString& strings) {
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kInsertMessageString)) {
    LOG4CXX_WARN(logger(), "Incorrect insert statement for message.");
    return false;
  }

  query.Bind(0, *strings.tts);
  query.Bind(1, *strings.label);
  query.Bind(2, *strings.line1);
  query.Bind(3, *strings.line2);
  query.Bind(4, lang);
  query.Bind(5, type);
  query.Bind(6, *strings.textBody);

  if (!query.Exec() || !query.Reset()) {
    LOG4CXX_WARN(logger(), "Incorrect insert into message.");
    return false;
  }

  return true;
}

}  // namespace policy
