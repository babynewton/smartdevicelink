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

#include <sstream>
#include "policy/sql_pt_representation.h"
#include "policy/sql_wrapper.h"
#include "policy/sql_pt_queries.h"

namespace policy {

namespace {
template<typename T, typename K> void InsertUnique(K value, T* array) {
  int i = 0;
  for (; i < array->size() && array->at(i) != value; ++i) {
    continue;
  }
  if (array->size() == i) {
    array->push_back(value);
  }
}
}  //  namespace

log4cxx::LoggerPtr SQLPTRepresentation::logger_ = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("SQLPTRepresentation"));

const std::string SQLPTRepresentation::kDatabaseName = "policy";

SQLPTRepresentation::SQLPTRepresentation()
  : db_(new dbms::SQLDatabase(kDatabaseName)) {
}

SQLPTRepresentation::~SQLPTRepresentation() {
  db_->Close();
  delete db_;
}

CheckPermissionResult SQLPTRepresentation::CheckPermissions(
  const PTString& app_id, const PTString& hmi_level, const PTString& rpc) {
  CheckPermissionResult result(false);
  dbms::SQLQuery query(db_);

  if (!query.Prepare(sql_pt::kSelectRpc)) {
    LOG4CXX_WARN(
      logger_,
      "Incorrect select statement from rpcs" << query.LastError().text());
    return result;
  }
  query.Bind(0, app_id);
  query.Bind(1, hmi_level);
  query.Bind(2, rpc);

  bool ret = query.Next();
  result.hmi_level_permitted = ret;
  LOG4CXX_INFO(
    logger_,
    "Level is "
    << (result.hmi_level_permitted ? "permitted" : "not permitted"));
  std::string parameter;
  while (ret) {
    if (!query.IsNull(0)) {
      if (!result.list_of_allowed_params) {
        result.list_of_allowed_params = new std::vector<PTString>();
      }
      parameter = query.GetString(0);
      result.list_of_allowed_params->push_back(parameter);
    }
    ret = query.Next();
  }

  return result;
}

bool SQLPTRepresentation::IsPTPreloaded() {
  dbms::SQLQuery query(db_);
  return query.Prepare(sql_pt::kSelectPreloaded) && query.Next();
}

int SQLPTRepresentation::IgnitionCyclesBeforeExchange() {
  dbms::SQLQuery query(db_);
  if (!query.Prepare(sql_pt::kSelectIgnitionCycles) || !query.Exec()) {
    LOG4CXX_WARN(logger_, "Can not select ignition cycles");
    return 0;
  }
  int limit = query.GetInteger(0);
  int current = query.GetInteger(1);

  if (limit < 0 || current < 0 || current > limit) {
    return 0;
  }

  return limit - current;
}

int SQLPTRepresentation::KilometersBeforeExchange(int current) {
  dbms::SQLQuery query(db_);
  if (!query.Prepare(sql_pt::kSelectKilometers) || !query.Exec()) {
    LOG4CXX_WARN(logger_, "Can not select kilometers");
    return 0;
  }
  int limit = query.GetInteger(0);
  int last = query.GetInteger(1);

  if (limit < 0 || last < 0 || current < 0 || current < last
      || limit < (current - last)) {
    return 0;
  }

  return limit - (current - last);
}

bool SQLPTRepresentation::SetCountersPassedForSuccessfulUpdate(
  int kilometers, int days_after_epoch) {
  LOG4CXX_INFO(logger_,
               "SQLPTRepresentation::SetCountersPassedForSuccessfulUpdate");
  dbms::SQLQuery query(db_);
  if (!query.Prepare(sql_pt::kUpdateCountersSuccessfulUpdate)) {
    LOG4CXX_WARN(logger_,
                 "Wrong update query for counters on successful update.");
    return false;
  }
  query.Bind(0, kilometers);
  query.Bind(1, days_after_epoch);
  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Failed to update counters on successful update.");
    return false;
  }
  return true;
}

int SQLPTRepresentation::DaysBeforeExchange(int current) {
  dbms::SQLQuery query(db_);
  if (!query.Prepare(sql_pt::kSelectDays) || !query.Exec()) {
    LOG4CXX_WARN(logger_, "Can not select days");
    return 0;
  }
  int limit = query.GetInteger(0);
  int last = query.GetInteger(1);

  if (0 == last) {
    return limit;
  }

  if (limit < 0 || last < 0 || current < 0 || current < last
      || limit < (current - last)) {
    return 0;
  }

  return limit - (current - last);
}

int SQLPTRepresentation::TimeoutResponse() {
  dbms::SQLQuery query(db_);
  if (!query.Prepare(sql_pt::kSelectTimeoutResponse) || !query.Exec()) {
    LOG4CXX_INFO(logger_, "Can not select timeout response for retry sequence");
    const int kDefault = 30;
    return kDefault;
  }
  return query.GetInteger(0);
}

bool SQLPTRepresentation::SecondsBetweenRetries(std::vector<int>* seconds) {
  dbms::SQLQuery query(db_);
  if (!query.Prepare(sql_pt::kSelectSecondsBetweenRetries)) {
    LOG4CXX_INFO(logger_,
                 "Incorrect select statement from seconds between retries");
    return false;
  }
  while (query.Next()) {
    seconds->push_back(query.GetInteger(0));
  }
  return true;
}

std::vector<UserFriendlyMessage> SQLPTRepresentation::GetUserFriendlyMsg(
  const std::vector<std::string>& msg_codes, const std::string& language) {
  return std::vector<UserFriendlyMessage>();
}

EndpointUrls SQLPTRepresentation::GetUpdateUrls(int service_type) {
  LOG4CXX_INFO(logger_, "SQLPTRepresentation::GetUpdateUrls for " << service_type);
  dbms::SQLQuery query(db_);
  EndpointUrls ret;
  if (query.Prepare(sql_pt::kSelectEndpoint)) {
    query.Bind(0, service_type);
    while (query.Next()) {
      EndpointData data;
      data.url = query.GetString(0);
      if (!query.IsNull(1)) {
        data.app_id = query.GetString(1);
      }
      ret.push_back(data);
    }
  } else {
    LOG4CXX_WARN(logger_, "Invalid select endpoints statement.");
  }
  return ret;
}

int SQLPTRepresentation::GetNotificationsNumber(
  policy_table::Priority priority) {
  return 0;
}

InitResult SQLPTRepresentation::Init() {
  LOG4CXX_INFO(logger_, "SQLPTRepresentation::Init");

  if (!db_->Open()) {
    LOG4CXX_INFO(logger_, "Failed opening database");
    return InitResult::FAIL;
  }
  dbms::SQLQuery check_pages(db_);
  if (!check_pages.Prepare(sql_pt::kCheckPgNumber) || !check_pages.Next()) {
    LOG4CXX_WARN(logger_, "Incorrect pragma for page counting.");
  } else {
    if (0 < check_pages.GetInteger(0)) {
      dbms::SQLQuery db_check(db_);
      if (!db_check.Prepare(sql_pt::kCheckDBIntegrity)) {
        LOG4CXX_WARN(logger_, "Incorrect pragma for integrity check.");
      } else {
        while (db_check.Next()) {
          if (db_check.GetString(0).compare("ok") == 0) {
            return InitResult::EXISTS;
          } else {
            LOG4CXX_ERROR(logger_,
                          "Existing policy table representation is invlaid.");
            // TODO(PV): add handle
            return InitResult::FAIL;
          }
        }
      }
    }
  }
  dbms::SQLQuery query(db_);
  if (!query.Exec(sql_pt::kCreateSchema)) {
    LOG4CXX_INFO(
      logger_,
      "Failed creating schema of database: " << query.LastError().text());
    return InitResult::FAIL;
  }
  return InitResult::SUCCESS;
}

bool SQLPTRepresentation::Close() {
  db_->Close();
  return db_->LastError().number() == dbms::OK;
}

VehicleData SQLPTRepresentation::GetVehicleData() {
  return VehicleData();
}

bool SQLPTRepresentation::Clear() {
  dbms::SQLQuery query(db_);
  if (!query.Exec(sql_pt::kDropSchema)) {
    LOG4CXX_WARN(logger_,
                 "Failed clearing database: " << query.LastError().text());
    return false;
  }
  return true;
}

utils::SharedPtr<policy_table::Table> SQLPTRepresentation::GenerateSnapshot() const {
  LOG4CXX_INFO(logger_, "GenerateSnapshot");
  utils::SharedPtr<policy_table::Table> table = new policy_table::Table();
  GatherModuleMeta(&table->policy_table.module_meta);
  GatherModuleConfig(&table->policy_table.module_config);
  GatherUsageAndErrorCounts(&table->policy_table.usage_and_error_counts);
  GatherDeviceData(&table->policy_table.device_data);
  GatherFunctionalGroupings(&table->policy_table.functional_groupings);
  GatherConsumerFriendlyMessages(
    &table->policy_table.consumer_friendly_messages);
  GatherApplicationPolicies(&table->policy_table.app_policies);
  return table;
}

void SQLPTRepresentation::GatherModuleMeta(
  policy_table::ModuleMeta* meta) const {
  LOG4CXX_INFO(logger_, "Gather Module Meta Info");
  // Section Module Meta is empty for SDL specific
}

void SQLPTRepresentation::GatherModuleConfig(
  policy_table::ModuleConfig* config) const {
  LOG4CXX_INFO(logger_, "Gather Configuration Info");
  dbms::SQLQuery query(db_);
  if (!query.Prepare(sql_pt::kSelectModuleConfig) || !query.Next()) {
    LOG4CXX_WARN(logger_, "Incorrect select statement for module config");
  } else {
    *config->preloaded_pt = query.GetBoolean(0);
    config->exchange_after_x_ignition_cycles = query.GetInteger(1);
    config->exchange_after_x_kilometers = query.GetInteger(2);
    config->exchange_after_x_days = query.GetInteger(3);
    config->timeout_after_x_seconds = query.GetInteger(4);
    *config->vehicle_make = query.GetString(5);
    *config->vehicle_model = query.GetString(6);
    *config->vehicle_year = query.GetInteger(7);
  }

  dbms::SQLQuery endpoints(db_);
  if (!endpoints.Prepare(sql_pt::kSelectEndpoints)) {
    LOG4CXX_WARN(logger_, "Incorrect select statement for endpoints");
  } else {
    while (endpoints.Next()) {
      std::stringstream stream;
      stream << "0x0" << endpoints.GetInteger(1);
      config->endpoints[endpoints.GetString(2)][stream.str()]
      .push_back(endpoints.GetString(0));
    }
  }

  dbms::SQLQuery notifications(db_);
  if (!notifications.Prepare(sql_pt::kSelectNotificationsPerMin)) {
    LOG4CXX_WARN(logger_, "Incorrect select statement for notifications");
  } else {
    while (notifications.Next()) {
      config->notifications_per_minute_by_priority[notifications.GetString(0)] =
        notifications.GetInteger(1);
    }
  }
  dbms::SQLQuery seconds(db_);
  if (!seconds.Prepare(sql_pt::kSelectSecondsBetweenRetries)) {
    LOG4CXX_INFO(logger_,
                 "Incorrect select statement from seconds between retries");
  } else {
    while (seconds.Next()) {
      config->seconds_between_retries.push_back(seconds.GetInteger(0));
    }
  }
}

bool SQLPTRepresentation::GatherUsageAndErrorCounts(
  policy_table::UsageAndErrorCounts* counts) const {
  LOG4CXX_INFO(logger_, "Gather Usage and Error Counts.");
  dbms::SQLQuery query(db_);
  if (query.Prepare(sql_pt::kSelectAppLevels)) {
    while (query.Next()) {
      counts->app_level[query.GetString(0)] = policy_table::AppLevel();
    }
  }
  return true;
}

void SQLPTRepresentation::GatherDeviceData(
  policy_table::DeviceData* data) const {
  LOG4CXX_INFO(logger_, "Gather device data.");
  dbms::SQLQuery query(db_);
  if (query.Prepare(sql_pt::kSelectDeviceData)) {
    while (query.Next()) {
      (*data)[query.GetString(0)] = policy_table::DeviceParams();
    }
  }
}

bool SQLPTRepresentation::GatherFunctionalGroupings(
  policy_table::FunctionalGroupings* groups) const {
  LOG4CXX_INFO(logger_, "Gather Functional Groupings info");
  dbms::SQLQuery func_group(db_);
  if (!func_group.Prepare(sql_pt::kSelectFunctionalGroups)) {
    LOG4CXX_WARN(logger_, "Incorrect select from functional_groupings");
    return false;
  }
  dbms::SQLQuery rpcs(db_);
  if (!rpcs.Prepare(sql_pt::kSelectAllRpcs)) {
    LOG4CXX_WARN(logger_, "Incorrect select all from rpc");
    return false;
  }
  while (func_group.Next()) {
    policy_table::Rpcs rpcs_tbl;
    if (!func_group.IsNull(2)) {
      *rpcs_tbl.user_consent_prompt = func_group.GetString(2);
    }
    int func_id = func_group.GetInteger(0);
    rpcs.Bind(0, func_id);
    while (rpcs.Next()) {
      if (!rpcs.IsNull(1)) {
        policy_table::HmiLevel level;
        policy_table::EnumFromJsonString(rpcs.GetString(1), &level);
        InsertUnique(level, &rpcs_tbl.rpcs[rpcs.GetString(0)].hmi_levels);
      }
      if (!rpcs.IsNull(2)) {
        policy_table::Parameter param;
        policy_table::EnumFromJsonString(rpcs.GetString(2), &param);
        InsertUnique(param, &(rpcs_tbl.rpcs[rpcs.GetString(0)].parameters));
      }
    }
    rpcs.Reset();
    (*groups)[func_group.GetString(1)] = rpcs_tbl;
  }
  return true;
}

bool SQLPTRepresentation::GatherConsumerFriendlyMessages(
  policy_table::ConsumerFriendlyMessages* messages) const {
  LOG4CXX_INFO(logger_, "Gather Consumer Friendly Messages");
  dbms::SQLQuery query(db_);
  if (!query.Prepare(sql_pt::kSelectUserMsgsVersion) || !query.Next()) {
    LOG4CXX_WARN(logger_, "Incorrect select from consumer_friendly_messages");
    return false;
  }
  messages->version = query.GetString(0);
  return true;
}

bool SQLPTRepresentation::GatherApplicationPolicies(
  policy_table::ApplicationPolicies* apps) const {
  LOG4CXX_INFO(logger_, "Gather applications policies");
  dbms::SQLQuery query(db_);
  if (!query.Prepare(sql_pt::kSelectAppPolicies)) {
    LOG4CXX_WARN(logger_, "Incorrect select from app_policies");
    return false;
  }

  while (query.Next()) {
    policy_table::ApplicationParams params;
    const std::string& app_id = query.GetString(0);
    params.memory_kb = query.GetInteger(1);
    params.watchdog_timer_ms = query.GetInteger(2);
    if (!query.IsNull(3)) {
      *params.certificate = query.GetString(3);
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
    (*apps)[app_id] = params;
  }
  return true;
}

bool SQLPTRepresentation::Save(const policy_table::Table& table) {
  LOG4CXX_INFO(logger_, "SQLPTRepresentation::Save");
  db_->BeginTransaction();
  if (!SaveFunctionalGroupings(table.policy_table.functional_groupings)) {
    db_->RollbackTransaction();
    return false;
  }
  if (!SaveApplicationPolicies(table.policy_table.app_policies)) {
    db_->RollbackTransaction();
    return false;
  }
  if (!SaveModuleConfig(table.policy_table.module_config)) {
    db_->RollbackTransaction();
    return false;
  }
  if (!SaveConsumerFriendlyMessages(
        table.policy_table.consumer_friendly_messages)) {
    db_->RollbackTransaction();
    return false;
  }
  if (!SaveDeviceData(table.policy_table.device_data)) {
    db_->RollbackTransaction();
    return false;
  }
  if (!SaveUsageAndErrorCounts(table.policy_table.usage_and_error_counts)) {
    db_->RollbackTransaction();
    return false;
  }
  if (!SaveModuleMeta(table.policy_table.module_meta)) {
    db_->RollbackTransaction();
    return false;
  }
  db_->CommitTransaction();
  return true;
}

bool SQLPTRepresentation::SaveFunctionalGroupings(
  const policy_table::FunctionalGroupings& groups) {
  LOG4CXX_INFO(logger_, "SaveFunctionalGroupings");
  dbms::SQLQuery query_delete(db_);
  if (!query_delete.Exec(sql_pt::kDeleteRpc)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from rpc.");
    return false;
  }

  dbms::SQLQuery query(db_);
  if (!query.Exec(sql_pt::kDeleteFunctionalGroup)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from seconds between retries.");
    return false;
  }
  if (!query.Prepare(sql_pt::kInsertFunctionalGroup)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for functional groups");
    return false;
  }

  policy_table::FunctionalGroupings::const_iterator it;
  for (it = groups.begin(); it != groups.end(); ++it) {
    query.Bind(0, it->first);
    it->second.user_consent_prompt.is_initialized() ?
    query.Bind(1, *(it->second.user_consent_prompt)) : query.Bind(1);

    if (!query.Exec() || !query.Reset()) {
      LOG4CXX_WARN(logger_, "Incorrect insert into functional groups");
      return false;
    }

    if (!SaveRpcs(query.LastInsertId(), it->second.rpcs)) {
      return false;
    }
  }
  return true;
}

bool SQLPTRepresentation::SaveRpcs(int64_t group_id,
                                   const policy_table::Rpc& rpcs) {
  dbms::SQLQuery query(db_);
  dbms::SQLQuery query_parameter(db_);
  if (!query.Prepare(sql_pt::kInsertRpc)
      || !query_parameter.Prepare(sql_pt::kInsertRpcWithParameter)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for rpc");
    return false;
  }

  policy_table::Rpc::const_iterator it;
  for (it = rpcs.begin(); it != rpcs.end(); ++it) {
    const policy_table::HmiLevels& hmi_levels = it->second.hmi_levels;
    const policy_table::Parameters& parameters = it->second.parameters;
    policy_table::HmiLevels::const_iterator hmi_it;
    policy_table::Parameters::const_iterator ps_it;
    for (hmi_it = hmi_levels.begin(); hmi_it != hmi_levels.end(); ++hmi_it) {
      if (!parameters.empty()) {
        for (ps_it = parameters.begin(); ps_it != parameters.end(); ++ps_it) {
          query_parameter.Bind(0, it->first);
          query_parameter.Bind(
            1, std::string(policy_table::EnumToJsonString(*hmi_it)));
          query_parameter.Bind(
            2, std::string(policy_table::EnumToJsonString(*ps_it)));
          query_parameter.Bind(3, group_id);
          if (!query_parameter.Exec() || !query_parameter.Reset()) {
            LOG4CXX_WARN(logger_, "Incorrect insert into rpc with parameter");
            return false;
          }
        }
      } else {
        query.Bind(0, it->first);
        query.Bind(1, std::string(policy_table::EnumToJsonString(*hmi_it)));
        query.Bind(2, group_id);
        if (!query.Exec() || !query.Reset()) {
          LOG4CXX_WARN(logger_, "Incorrect insert into rpc");
          return false;
        }
      }
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveApplicationPolicies(
  const policy_table::ApplicationPolicies& apps) {
  LOG4CXX_INFO(logger_, "SaveApplicationPolicies");
  dbms::SQLQuery query_delete(db_);
  if (!query_delete.Exec(sql_pt::kDeleteAppGroup)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from app_group.");
    return false;
  }
  if (!query_delete.Exec(sql_pt::kDeleteApplication)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from application.");
    return false;
  }
  policy_table::ApplicationPolicies::const_iterator it;
  dbms::SQLQuery app_query(db_);
  if (!app_query.Prepare(sql_pt::kInsertApplication)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement into application.");
    return false;
  }
  for (it = apps.begin(); it != apps.end(); ++it) {
    app_query.Bind(0, it->first);
    app_query.Bind(1, it->second.is_null());
    app_query.Bind(2, it->second.memory_kb);
    app_query.Bind(3, it->second.watchdog_timer_ms);
    it->second.certificate.is_initialized() ?
    app_query.Bind(4, it->second.certificate) : app_query.Bind(4);

    if (!app_query.Exec() || !app_query.Reset()) {
      LOG4CXX_WARN(logger_, "Incorrect insert into application.");
      return false;
    }

    LOG4CXX_INFO(logger_, "Saving data for application: " << it->first);
    if (!SaveAppGroup(it->first, it->second.groups)) {
      return false;
    }
    if (!SaveNickname(it->first, it->second.nicknames)) {
      return false;
    }
    if (!SaveAppType(it->first, it->second.AppHMIType)) {
      return false;
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveAppGroup(
  const std::string& app_id, const policy_table::Strings& app_groups) {
  dbms::SQLQuery query(db_);
  if (!query.Prepare(sql_pt::kInsertAppGroup)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for app group");
    return false;
  }
  LOG4CXX_INFO(logger_, "SaveAppGroup");
  policy_table::Strings::const_iterator it;
  for (it = app_groups.begin(); it != app_groups.end(); ++it) {
    std::string ssss = *it;
    LOG4CXX_INFO(logger_, "Group: " << ssss);
    query.Bind(0, app_id);
    query.Bind(1, *it);
    if (!query.Exec() || !query.Reset()) {
      LOG4CXX_WARN(
        logger_,
        "Incorrect insert into app group." << query.LastError().text());
      return false;
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveNickname(const std::string& app_id,
                                       const policy_table::Strings& nicknames) {
  dbms::SQLQuery query(db_);
  if (!query.Prepare(sql_pt::kInsertNickname)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for nickname");
    return false;
  }

  policy_table::Strings::const_iterator it;
  for (it = nicknames.begin(); it != nicknames.end(); ++it) {
    query.Bind(0, app_id);
    query.Bind(1, *it);
    if (!query.Exec() || !query.Reset()) {
      LOG4CXX_WARN(logger_, "Incorrect insert into nickname.");
      return false;
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveAppType(const std::string& app_id,
                                      const policy_table::AppHMITypes& types) {
  dbms::SQLQuery query(db_);
  if (!query.Prepare(sql_pt::kInsertAppType)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for app type");
    return false;
  }

  policy_table::AppHMITypes::const_iterator it;
  for (it = types.begin(); it != types.end(); ++it) {
    query.Bind(0, app_id);
    query.Bind(1, std::string(policy_table::EnumToJsonString(*it)));
    if (!query.Exec() || !query.Reset()) {
      LOG4CXX_WARN(logger_, "Incorrect insert into app type.");
      return false;
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveModuleMeta(const policy_table::ModuleMeta& meta) {
  LOG4CXX_INFO(logger_, "SaveModuleMeta");
  // Section Module Meta is empty for SDL specific
  return true;
}

bool SQLPTRepresentation::SaveModuleConfig(
  const policy_table::ModuleConfig& config) {
  LOG4CXX_INFO(logger_, "SaveModuleConfig");
  dbms::SQLQuery query(db_);
  if (!query.Prepare(sql_pt::kUpdateModuleConfig)) {
    LOG4CXX_WARN(logger_, "Incorrect update statement for module config");
    return false;
  }

  config.preloaded_pt.is_initialized() ?
  query.Bind(0, config.preloaded_pt) : query.Bind(0, false);
  query.Bind(1, config.exchange_after_x_ignition_cycles);
  query.Bind(2, config.exchange_after_x_kilometers);
  query.Bind(3, config.exchange_after_x_days);
  query.Bind(4, config.timeout_after_x_seconds);
  config.vehicle_make.is_initialized() ?
  query.Bind(5, *(config.vehicle_make)) : query.Bind(5);
  config.vehicle_model.is_initialized() ?
  query.Bind(6, *(config.vehicle_model)) : query.Bind(6);
  config.vehicle_year.is_initialized() ?
  query.Bind(7, *(config.vehicle_year)) : query.Bind(7);

  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Incorrect update module config");
    return false;
  }

  if (!SaveSecondsBetweenRetries(config.seconds_between_retries)) {
    return false;
  }

  if (!SaveNumberOfNotificationsPerMinute(
        config.notifications_per_minute_by_priority)) {
    return false;
  }

  if (!SaveServiceEndpoints(config.endpoints)) {
    return false;
  }

  return true;
}

bool SQLPTRepresentation::SaveServiceEndpoints(
  const policy_table::ServiceEndpoints& endpoints) {
  dbms::SQLQuery query(db_);
  if (!query.Exec(sql_pt::kDeleteEndpoint)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from endpoint.");
    return false;
  }

  if (!query.Prepare(sql_pt::kInsertEndpoint)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for endpoint");
    return false;
  }

  policy_table::ServiceEndpoints::const_iterator it;
  for (it = endpoints.begin(); it != endpoints.end(); ++it) {
    const policy_table::URLList& apps = it->second;
    policy_table::URLList::const_iterator app_it;
    for (app_it = apps.begin(); app_it != apps.end(); ++app_it) {
      const policy_table::URL& urls = app_it->second;
      policy_table::URL::const_iterator url_it;
      for (url_it = urls.begin(); url_it != urls.end(); ++url_it) {
        std::stringstream temp_stream(it->first);
        int service;
        temp_stream.seekg(3);
        temp_stream >> service;
        query.Bind(0, service);
        query.Bind(1, *url_it);
        query.Bind(2, app_it->first);
        if (!query.Exec()) {
          LOG4CXX_WARN(logger_, "Incorrect insert into endpoint");
          return false;
        }
      }
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveConsumerFriendlyMessages(
  const policy_table::ConsumerFriendlyMessages& messages) {
  LOG4CXX_INFO(logger_, "SaveConsumerFriendlyMessages");

  dbms::SQLQuery query(db_);
  if (!query.Exec(sql_pt::kDeleteMessageString)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from message.");
    return false;
  }

  if (query.Prepare(sql_pt::kUpdateVersion)) {
    query.Bind(0, messages.version);
    if (!query.Exec()) {
      LOG4CXX_WARN(logger_, "Incorrect update into version.");
      return false;
    }
  } else {
    LOG4CXX_WARN(logger_, "Incorrect update statement for version.");
    return false;
  }

  policy_table::Messages::const_iterator it;
  for (it = messages.messages.begin(); it != messages.messages.end(); ++it) {
    if (!SaveMessageType(it->first)) {
      return false;
    }
    const policy_table::Languages& langs = it->second.languages;
    policy_table::Languages::const_iterator lang_it;
    for (lang_it = langs.begin(); lang_it != langs.end(); ++lang_it) {
      if (!SaveLanguage(lang_it->first)) {
        return false;
      }
      if (!SaveMessageString(it->first, lang_it->first, lang_it->second)) {
        return false;
      }
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveMessageType(const std::string& type) {
  dbms::SQLQuery query(db_);
  if (!query.Prepare(sql_pt::kInsertMessageType)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for message type.");
    return false;
  }

  query.Bind(0, type);
  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Incorrect insert into message type.");
    return false;
  }

  return true;
}

bool SQLPTRepresentation::SaveLanguage(const std::string& code) {
  dbms::SQLQuery query(db_);
  if (!query.Prepare(sql_pt::kInsertLanguage)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for language.");
    return false;
  }

  query.Bind(0, code);
  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Incorrect insert into language.");
    return false;
  }

  return true;
}

bool SQLPTRepresentation::SaveMessageString(
  const std::string& type, const std::string& lang,
  const policy_table::MessageString& strings) {
  // Section is empty for SDL specific
  return true;
}

bool SQLPTRepresentation::SaveSecondsBetweenRetries(
  const policy_table::SecondsBetweenRetries& seconds) {
  dbms::SQLQuery query(db_);
  if (!query.Exec(sql_pt::kDeleteSecondsBetweenRetries)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from seconds between retries.");
    return false;
  }
  if (!query.Prepare(sql_pt::kInsertSecondsBetweenRetry)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect insert statement for seconds between retries.");
    return false;
  }

  for (int i = 0; i < seconds.size(); ++i) {
    query.Bind(0, i);
    query.Bind(1, seconds[i]);
    if (!query.Exec() || !query.Reset()) {
      LOG4CXX_WARN(logger_, "Incorrect insert into seconds between retries.");
      return false;
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveNumberOfNotificationsPerMinute(
  const policy_table::NumberOfNotificationsPerMinute& notifications) {
  dbms::SQLQuery query(db_);
  if (!query.Prepare(sql_pt::kInsertNotificationsByPriority)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect insert statement for notifications by priority.");
    return false;
  }

  policy_table::NumberOfNotificationsPerMinute::const_iterator it;
  for (it = notifications.begin(); it != notifications.end(); ++it) {
    query.Bind(0, it->first);
    query.Bind(1, it->second);
    if (!query.Exec() || !query.Reset()) {
      LOG4CXX_WARN(logger_, "Incorrect insert into notifications by priority.");
      return false;
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveDeviceData(
  const policy_table::DeviceData& devices) {
  LOG4CXX_INFO(logger_, "SaveDeviceData");
  dbms::SQLQuery query(db_);
  if (!query.Prepare(sql_pt::kInsertDeviceData)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for device data.");
    return false;
  }

  policy_table::DeviceData::const_iterator it;
  for (it = devices.begin(); it != devices.end(); ++it) {
    query.Bind(0, it->first);
    if (!query.Exec()) {
      LOG4CXX_WARN(logger_, "Incorrect insert into device data.");
      return false;
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveUsageAndErrorCounts(
  const policy_table::UsageAndErrorCounts& counts) {
  LOG4CXX_INFO(logger_, "SaveUsageAndErrorCounts");
  dbms::SQLQuery query(db_);
  if (!query.Exec(sql_pt::kDeleteAppLevel)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from app level.");
    return false;
  }
  if (!query.Prepare(sql_pt::kInsertAppLevel)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for app level.");
    return false;
  }

  policy_table::AppLevels::const_iterator it;
  for (it = counts.app_level.begin(); it != counts.app_level.end(); ++it) {
    query.Bind(0, it->first);
    if (!query.Exec()) {
      LOG4CXX_WARN(logger_, "Incorrect insert into app level.");
      return false;
    }
  }

  return true;
}

void SQLPTRepresentation::IncrementIgnitionCycles() {
  dbms::SQLQuery query(db_);
  if (!query.Exec(sql_pt::kIncrementIgnitionCycles)) {
    LOG4CXX_WARN(logger_, "Failed incrementing ignition cycles");
  }
}

void SQLPTRepresentation::ResetIgnitionCycles() {
  LOG4CXX_INFO(logger_, "ResetIgnitionCycles");
  dbms::SQLQuery query(db_);
  if (!query.Exec(sql_pt::kResetIgnitionCycles)) {
    LOG4CXX_WARN(logger_, "Failed to reset ignition cycles number.");
  }
}

bool SQLPTRepresentation::UpdateRequired() const {
  dbms::SQLQuery query(db_);
  if (!query.Prepare(sql_pt::kSelectFlagUpdateRequired) || !query.Exec()) {
    LOG4CXX_WARN(logger_,
                 "Failed select update required flag from module meta");
    return false;
  }
  return query.GetBoolean(0);
}

void SQLPTRepresentation::SaveUpdateRequired(bool value) {
  dbms::SQLQuery query(db_);
  if (!query.Prepare(sql_pt::kUpdateFlagUpdateRequired)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect update into module meta (update_required)");
    return;
  }
  query.Bind(0, value);
  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Failed update module meta (update_required)");
  }
}

bool SQLPTRepresentation::GetInitialAppData(const std::string& app_id,
    StringArray* nicknames,
    StringArray* app_types) {
  LOG4CXX_INFO(logger_, "Getting initial application data.");
  dbms::SQLQuery app_names(db_);
  if (!app_names.Prepare(sql_pt::kSelectNicknames)) {
    LOG4CXX_WARN(logger_, "Incorrect select from app nicknames");
    return false;
  }
  dbms::SQLQuery app_hmi_types(db_);
  if (!app_hmi_types.Prepare(sql_pt::kSelectAppTypes)) {
    LOG4CXX_WARN(logger_, "Incorrect select from app types");
    return false;
  }
  app_names.Bind(0, app_id);
  while (app_names.Next()) {
    nicknames->push_back(app_names.GetString(0));
  }
  app_names.Reset();
  app_hmi_types.Bind(0, app_id);
  while (app_hmi_types.Next()) {
    app_types->push_back(app_names.GetString(0));
  }
  app_hmi_types.Reset();
  return true;
}

bool SQLPTRepresentation::GetFunctionalGroupings(
  policy_table::FunctionalGroupings& groups) {
  LOG4CXX_INFO(logger(), "GetFunctionalGroupings");
  return GatherFunctionalGroupings(&groups);
}

bool SQLPTRepresentation::GatherAppType(
  const std::string& app_id, policy_table::AppHMITypes* app_types) const {
  dbms::SQLQuery query(db_);
  if (!query.Prepare(sql_pt::kSelectAppTypes)) {
    LOG4CXX_WARN(logger_, "Incorrect select from app types");
    return false;
  }

  query.Bind(0, app_id);
  while (query.Next()) {
    policy_table::AppHMIType type;
    if (!policy_table::EnumFromJsonString(query.GetString(0), &type)) {
      return false;
    }
    app_types->push_back(type);
  }
  return true;
}

bool SQLPTRepresentation::GatherNickName(
  const std::string& app_id, policy_table::Strings* nicknames) const {
  dbms::SQLQuery query(db_);
  if (!query.Prepare(sql_pt::kSelectNicknames)) {
    LOG4CXX_WARN(logger_, "Incorrect select from app nicknames");
    return false;
  }

  query.Bind(0, app_id);
  while (query.Next()) {
    nicknames->push_back(query.GetString(0));
  }
  return true;
}

bool SQLPTRepresentation::GatherAppGroup(
  const std::string& app_id, policy_table::Strings* app_groups) const {
  dbms::SQLQuery query(db_);
  if (!query.Prepare(sql_pt::kSelectAppGroups)) {
    LOG4CXX_WARN(logger_, "Incorrect select from app groups");
    return false;
  }

  query.Bind(0, app_id);
  while (query.Next()) {
    app_groups->push_back(query.GetString(0));
  }
  return true;
}

bool SQLPTRepresentation::IsApplicationRevoked(
  const std::string& app_id) const {
  dbms::SQLQuery query(db_);
  if (!query.Prepare(sql_pt::kSelectApplicationRevoked)) {
    LOG4CXX_WARN(logger_, "Incorrect select from is_revoked of application");
    return false;
  }

  query.Bind(0, app_id);
  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Failed select is_revoked of application");
    return false;
  }
  return query.IsNull(0) ? false : query.GetBoolean(0);
}

bool SQLPTRepresentation::IsApplicationRepresented(
  const std::string& app_id) const {
  dbms::SQLQuery query(db_);
  if (!query.Prepare(sql_pt::kSelectApplicationRepresented)) {
    LOG4CXX_WARN(logger_, "Incorrect select application by id");
    return false;
  }

  query.Bind(0, app_id);
  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Failed select application by id");
    return false;
  }
  return query.GetInteger(0) != 0;
}

bool SQLPTRepresentation::IsDefaultPolicy(const std::string& app_id) const {
  dbms::SQLQuery query(db_);
  if (!query.Prepare(sql_pt::kSelectApplicationIsDefault)) {
    LOG4CXX_WARN(logger_, "Incorrect select application by id");
    return false;
  }

  query.Bind(0, app_id);
  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Failed select application by id");
    return false;
  }
  return query.IsNull(0) ? false : query.GetBoolean(0);
}

bool SQLPTRepresentation::SetDefaultPolicy(const std::string& app_id) {
  policy_table::ApplicationPolicies apps;
  if (!GatherApplicationPolicies(&apps)) {
    LOG4CXX_WARN(logger_, "Failed gathering application policies");
    return false;
  }
  apps[app_id] = apps["default"];
  if (!SaveApplicationPolicies(apps)) {
    LOG4CXX_WARN(logger_, "Failed saving application policies");
    return false;
  }

  return SetIsDefault(app_id, true);
}

bool SQLPTRepresentation::SetIsDefault(const std::string& app_id,
                                       bool is_default) {
  LOG4CXX_TRACE(logger_, "Set flag is_default of application");
  dbms::SQLQuery query(db_);
  if (!query.Prepare(sql_pt::kUpdateIsDefault)) {
    LOG4CXX_WARN(logger_, "Incorect statement for updatign is_default");
    return false;
  }

  query.Bind(0, is_default);
  query.Bind(1, app_id);
  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Failed update is_default");
    return false;
  }
  return true;
}

}  // namespace policy

