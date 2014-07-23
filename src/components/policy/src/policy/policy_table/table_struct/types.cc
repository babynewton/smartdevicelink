// This file is generated, do not edit
#include "./types.h"
#include "rpc_base/rpc_base_json_inl.h"

namespace rpc {
namespace policy_table_interface_base {
// ApplicationParams methods
ApplicationParams::ApplicationParams()
  : CompositeType(kUninitialized) {
}
ApplicationParams::ApplicationParams(const Strings& groups)
  : CompositeType(kUninitialized),
    groups(groups) {
}
ApplicationParams::~ApplicationParams() {
}
ApplicationParams::ApplicationParams(const Json::Value* value__)
  : CompositeType(InitHelper(value__, &Json::Value::isObject)),
    groups(impl::ValueMember(value__, "groups")),
    nicknames(impl::ValueMember(value__, "nicknames")),
    AppHMIType(impl::ValueMember(value__, "AppHMIType")),
    memory_kb(impl::ValueMember(value__, "memory_kb")),
    heart_beat_timeout_ms(impl::ValueMember(value__, "heart_beat_timeout_ms")),
    certificate(impl::ValueMember(value__, "certificate")) {
}
Json::Value ApplicationParams::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("groups", groups, &result__);
  impl::WriteJsonField("nicknames", nicknames, &result__);
  impl::WriteJsonField("AppHMIType", AppHMIType, &result__);
  impl::WriteJsonField("memory_kb", memory_kb, &result__);
  impl::WriteJsonField("heart_beat_timeout_ms", heart_beat_timeout_ms, &result__);
  impl::WriteJsonField("certificate", certificate, &result__);
  return result__;
}
bool ApplicationParams::is_valid() const {
  if (!groups.is_valid()) {
    return false;
  }
  if (!nicknames.is_valid()) {
    return false;
  }
  if (!AppHMIType.is_valid()) {
    return false;
  }
  if (!memory_kb.is_valid()) {
    return false;
  }
  if (!heart_beat_timeout_ms.is_valid()) {
    return false;
  }
  if (!certificate.is_valid()) {
    return false;
  }
  return Validate();
}
bool ApplicationParams::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}
bool ApplicationParams::struct_empty() const {
  if (groups.is_initialized()) {
    return false;
  }
  if (nicknames.is_initialized()) {
    return false;
  }

  if (AppHMIType.is_initialized()) {
    return false;
  }
  if (memory_kb.is_initialized()) {
    return false;
  }

  if (heart_beat_timeout_ms.is_initialized()) {
    return false;
  }
  if (certificate.is_initialized()) {
    return false;
  }

  return true;
}
void ApplicationParams::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (!groups.is_valid()) {
    groups.ReportErrors(&report__->ReportSubobject("groups"));
  }
  if (!nicknames.is_valid()) {
    nicknames.ReportErrors(&report__->ReportSubobject("nicknames"));
  }
  if (!AppHMIType.is_valid()) {
    AppHMIType.ReportErrors(&report__->ReportSubobject("AppHMIType"));
  }
  if (!memory_kb.is_valid()) {
    memory_kb.ReportErrors(&report__->ReportSubobject("memory_kb"));
  }
  if (!heart_beat_timeout_ms.is_valid()) {
    heart_beat_timeout_ms.ReportErrors(&report__->ReportSubobject("heart_beat_timeout_ms"));
  }
  if (!certificate.is_valid()) {
    certificate.ReportErrors(&report__->ReportSubobject("certificate"));
  }
}

// RpcParameters methods
RpcParameters::RpcParameters()
  : CompositeType(kUninitialized) {
}
RpcParameters::RpcParameters(const HmiLevels& hmi_levels)
  : CompositeType(kUninitialized),
    hmi_levels(hmi_levels) {
}
RpcParameters::~RpcParameters() {
}
RpcParameters::RpcParameters(const Json::Value* value__)
  : CompositeType(InitHelper(value__, &Json::Value::isObject)),
    hmi_levels(impl::ValueMember(value__, "hmi_levels")),
    parameters(impl::ValueMember(value__, "parameters")) {
}
Json::Value RpcParameters::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("hmi_levels", hmi_levels, &result__);
  impl::WriteJsonField("parameters", parameters, &result__);
  return result__;
}
bool RpcParameters::is_valid() const {
  if (!hmi_levels.is_valid()) {
    return false;
  }
  if (!parameters.is_valid()) {
    return false;
  }
  return Validate();
}
bool RpcParameters::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}
bool RpcParameters::struct_empty() const {
  if (hmi_levels.is_initialized()) {
    return false;
  }
  if (parameters.is_initialized()) {
    return false;
  }

  return true;
}
void RpcParameters::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (!hmi_levels.is_valid()) {
    hmi_levels.ReportErrors(&report__->ReportSubobject("hmi_levels"));
  }
  if (!parameters.is_valid()) {
    parameters.ReportErrors(&report__->ReportSubobject("parameters"));
  }
}

// Rpcs methods
Rpcs::Rpcs()
  : CompositeType(kUninitialized) {
}
Rpcs::Rpcs(const Rpc& rpcs)
  : CompositeType(kUninitialized),
    rpcs(rpcs) {
}
Rpcs::~Rpcs() {
}
Rpcs::Rpcs(const Json::Value* value__)
  : CompositeType(InitHelper(value__, &Json::Value::isObject)),
    user_consent_prompt(impl::ValueMember(value__, "user_consent_prompt")),
    rpcs(impl::ValueMember(value__, "rpcs")) {
}
Json::Value Rpcs::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("user_consent_prompt", user_consent_prompt, &result__);
  impl::WriteJsonField("rpcs", rpcs, &result__);
  return result__;
}
bool Rpcs::is_valid() const {
  if (!user_consent_prompt.is_valid()) {
    return false;
  }
  if (!rpcs.is_valid()) {
    return false;
  }
  return Validate();
}
bool Rpcs::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}
bool Rpcs::struct_empty() const {
  if (user_consent_prompt.is_initialized()) {
    return false;
  }
  if (rpcs.is_initialized()) {
    return false;
  }

  return true;
}
void Rpcs::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (!user_consent_prompt.is_valid()) {
    user_consent_prompt.ReportErrors(&report__->ReportSubobject("user_consent_prompt"));
  }
  if (!rpcs.is_valid()) {
    rpcs.ReportErrors(&report__->ReportSubobject("rpcs"));
  }
}

// ModuleConfig methods
ModuleConfig::ModuleConfig()
  : CompositeType(kUninitialized) {
}
ModuleConfig::ModuleConfig(uint8_t exchange_after_x_ignition_cycles, int64_t exchange_after_x_kilometers, uint8_t exchange_after_x_days, uint16_t timeout_after_x_seconds, const SecondsBetweenRetries& seconds_between_retries, const ServiceEndpoints& endpoints, const NumberOfNotificationsPerMinute& notifications_per_minute_by_priority)
  : CompositeType(kUninitialized),
    exchange_after_x_ignition_cycles(exchange_after_x_ignition_cycles),
    exchange_after_x_kilometers(exchange_after_x_kilometers),
    exchange_after_x_days(exchange_after_x_days),
    timeout_after_x_seconds(timeout_after_x_seconds),
    seconds_between_retries(seconds_between_retries),
    endpoints(endpoints),
    notifications_per_minute_by_priority(notifications_per_minute_by_priority) {
}
ModuleConfig::~ModuleConfig() {
}
ModuleConfig::ModuleConfig(const Json::Value* value__)
  : CompositeType(InitHelper(value__, &Json::Value::isObject)),
    device_certificates(impl::ValueMember(value__, "device_certificates")),
    preloaded_pt(impl::ValueMember(value__, "preloaded_pt")),
    exchange_after_x_ignition_cycles(impl::ValueMember(value__, "exchange_after_x_ignition_cycles")),
    exchange_after_x_kilometers(impl::ValueMember(value__, "exchange_after_x_kilometers")),
    exchange_after_x_days(impl::ValueMember(value__, "exchange_after_x_days")),
    timeout_after_x_seconds(impl::ValueMember(value__, "timeout_after_x_seconds")),
    seconds_between_retries(impl::ValueMember(value__, "seconds_between_retries")),
    endpoints(impl::ValueMember(value__, "endpoints")),
    notifications_per_minute_by_priority(impl::ValueMember(value__, "notifications_per_minute_by_priority")),
    vehicle_make(impl::ValueMember(value__, "vehicle_make")),
    vehicle_model(impl::ValueMember(value__, "vehicle_model")),
    vehicle_year(impl::ValueMember(value__, "vehicle_year")) {
}
Json::Value ModuleConfig::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("device_certificates", device_certificates, &result__);
  impl::WriteJsonField("preloaded_pt", preloaded_pt, &result__);
  impl::WriteJsonField("exchange_after_x_ignition_cycles", exchange_after_x_ignition_cycles, &result__);
  impl::WriteJsonField("exchange_after_x_kilometers", exchange_after_x_kilometers, &result__);
  impl::WriteJsonField("exchange_after_x_days", exchange_after_x_days, &result__);
  impl::WriteJsonField("timeout_after_x_seconds", timeout_after_x_seconds, &result__);
  impl::WriteJsonField("seconds_between_retries", seconds_between_retries, &result__);
  impl::WriteJsonField("endpoints", endpoints, &result__);
  impl::WriteJsonField("notifications_per_minute_by_priority", notifications_per_minute_by_priority, &result__);
  impl::WriteJsonField("vehicle_make", vehicle_make, &result__);
  impl::WriteJsonField("vehicle_model", vehicle_model, &result__);
  impl::WriteJsonField("vehicle_year", vehicle_year, &result__);
  return result__;
}
bool ModuleConfig::is_valid() const {
  if (!device_certificates.is_valid()) {
    return false;
  }
  if (!preloaded_pt.is_valid()) {
    return false;
  }
  if (!exchange_after_x_ignition_cycles.is_valid()) {
    return false;
  }
  if (!exchange_after_x_kilometers.is_valid()) {
    return false;
  }
  if (!exchange_after_x_days.is_valid()) {
    return false;
  }
  if (!timeout_after_x_seconds.is_valid()) {
    return false;
  }
  if (!seconds_between_retries.is_valid()) {
    return false;
  }
  if (!endpoints.is_valid()) {
    return false;
  }
  if (!notifications_per_minute_by_priority.is_valid()) {
    return false;
  }
  if (!vehicle_make.is_valid()) {
    return false;
  }
  if (!vehicle_model.is_valid()) {
    return false;
  }
  if (!vehicle_year.is_valid()) {
    return false;
  }
  return Validate();
}
bool ModuleConfig::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}
bool ModuleConfig::struct_empty() const {
  if (device_certificates.is_initialized()) {
    return false;
  }
  if (preloaded_pt.is_initialized()) {
    return false;
  }

  if (exchange_after_x_ignition_cycles.is_initialized()) {
    return false;
  }
  if (exchange_after_x_kilometers.is_initialized()) {
    return false;
  }

  if (exchange_after_x_days.is_initialized()) {
    return false;
  }
  if (timeout_after_x_seconds.is_initialized()) {
    return false;
  }

  if (seconds_between_retries.is_initialized()) {
    return false;
  }
  if (endpoints.is_initialized()) {
    return false;
  }

  if (notifications_per_minute_by_priority.is_initialized()) {
    return false;
  }
  if (vehicle_make.is_initialized()) {
    return false;
  }

  if (vehicle_model.is_initialized()) {
    return false;
  }
  if (vehicle_year.is_initialized()) {
    return false;
  }

  return true;
}
void ModuleConfig::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (!device_certificates.is_valid()) {
    device_certificates.ReportErrors(&report__->ReportSubobject("device_certificates"));
  }
  if (!preloaded_pt.is_valid()) {
    preloaded_pt.ReportErrors(&report__->ReportSubobject("preloaded_pt"));
  }
  if (!exchange_after_x_ignition_cycles.is_valid()) {
    exchange_after_x_ignition_cycles.ReportErrors(&report__->ReportSubobject("exchange_after_x_ignition_cycles"));
  }
  if (!exchange_after_x_kilometers.is_valid()) {
    exchange_after_x_kilometers.ReportErrors(&report__->ReportSubobject("exchange_after_x_kilometers"));
  }
  if (!exchange_after_x_days.is_valid()) {
    exchange_after_x_days.ReportErrors(&report__->ReportSubobject("exchange_after_x_days"));
  }
  if (!timeout_after_x_seconds.is_valid()) {
    timeout_after_x_seconds.ReportErrors(&report__->ReportSubobject("timeout_after_x_seconds"));
  }
  if (!seconds_between_retries.is_valid()) {
    seconds_between_retries.ReportErrors(&report__->ReportSubobject("seconds_between_retries"));
  }
  if (!endpoints.is_valid()) {
    endpoints.ReportErrors(&report__->ReportSubobject("endpoints"));
  }
  if (!notifications_per_minute_by_priority.is_valid()) {
    notifications_per_minute_by_priority.ReportErrors(&report__->ReportSubobject("notifications_per_minute_by_priority"));
  }
  if (!vehicle_make.is_valid()) {
    vehicle_make.ReportErrors(&report__->ReportSubobject("vehicle_make"));
  }
  if (!vehicle_model.is_valid()) {
    vehicle_model.ReportErrors(&report__->ReportSubobject("vehicle_model"));
  }
  if (!vehicle_year.is_valid()) {
    vehicle_year.ReportErrors(&report__->ReportSubobject("vehicle_year"));
  }
}

// MessageString methods
MessageString::MessageString()
  : CompositeType(kUninitialized) {
}
MessageString::~MessageString() {
}
MessageString::MessageString(const Json::Value* value__)
  : CompositeType(InitHelper(value__, &Json::Value::isObject)),
    line1(impl::ValueMember(value__, "line1")),
    line2(impl::ValueMember(value__, "line2")),
    tts(impl::ValueMember(value__, "tts")),
    label(impl::ValueMember(value__, "label")),
    textBody(impl::ValueMember(value__, "textBody")) {
}
Json::Value MessageString::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("line1", line1, &result__);
  impl::WriteJsonField("line2", line2, &result__);
  impl::WriteJsonField("tts", tts, &result__);
  impl::WriteJsonField("label", label, &result__);
  impl::WriteJsonField("textBody", textBody, &result__);
  return result__;
}
bool MessageString::is_valid() const {
  if (struct_empty()) {
    return initialization_state__ == kInitialized;
  }
  if (!line1.is_valid()) {
    return false;
  }
  if (!line2.is_valid()) {
    return false;
  }
  if (!tts.is_valid()) {
    return false;
  }
  if (!label.is_valid()) {
    return false;
  }
  if (!textBody.is_valid()) {
    return false;
  }
  return Validate();
}
bool MessageString::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}
bool MessageString::struct_empty() const {
  if (line1.is_initialized()) {
    return false;
  }
  if (line2.is_initialized()) {
    return false;
  }

  if (tts.is_initialized()) {
    return false;
  }
  if (label.is_initialized()) {
    return false;
  }

  if (textBody.is_initialized()) {
    return false;
  }
  return true;
}
void MessageString::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (!line1.is_valid()) {
    line1.ReportErrors(&report__->ReportSubobject("line1"));
  }
  if (!line2.is_valid()) {
    line2.ReportErrors(&report__->ReportSubobject("line2"));
  }
  if (!tts.is_valid()) {
    tts.ReportErrors(&report__->ReportSubobject("tts"));
  }
  if (!label.is_valid()) {
    label.ReportErrors(&report__->ReportSubobject("label"));
  }
  if (!textBody.is_valid()) {
    textBody.ReportErrors(&report__->ReportSubobject("textBody"));
  }
}

// MessageLanguages methods
MessageLanguages::MessageLanguages()
  : CompositeType(kUninitialized) {
}
MessageLanguages::MessageLanguages(const Languages& languages)
  : CompositeType(kUninitialized),
    languages(languages) {
}
MessageLanguages::~MessageLanguages() {
}
MessageLanguages::MessageLanguages(const Json::Value* value__)
  : CompositeType(InitHelper(value__, &Json::Value::isObject)),
    languages(impl::ValueMember(value__, "languages")) {
}
Json::Value MessageLanguages::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("languages", languages, &result__);
  return result__;
}
bool MessageLanguages::is_valid() const {
  if (!languages.is_valid()) {
    return false;
  }
  return Validate();
}
bool MessageLanguages::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}
bool MessageLanguages::struct_empty() const {
  if (languages.is_initialized()) {
    return false;
  }
  return true;
}
void MessageLanguages::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (!languages.is_valid()) {
    languages.ReportErrors(&report__->ReportSubobject("languages"));
  }
}

// ConsumerFriendlyMessages methods
ConsumerFriendlyMessages::ConsumerFriendlyMessages()
  : CompositeType(kUninitialized) {
}
ConsumerFriendlyMessages::ConsumerFriendlyMessages(const std::string& version)
  : CompositeType(kUninitialized),
    version(version) {
}
ConsumerFriendlyMessages::~ConsumerFriendlyMessages() {
}
ConsumerFriendlyMessages::ConsumerFriendlyMessages(const Json::Value* value__)
  : CompositeType(InitHelper(value__, &Json::Value::isObject)),
    version(impl::ValueMember(value__, "version")),
    messages(impl::ValueMember(value__, "messages")) {
}
Json::Value ConsumerFriendlyMessages::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("version", version, &result__);
  impl::WriteJsonField("messages", messages, &result__);
  return result__;
}
bool ConsumerFriendlyMessages::is_valid() const {
  if (!version.is_valid()) {
    return false;
  }
  if (!messages.is_valid()) {
    return false;
  }
  return Validate();
}
bool ConsumerFriendlyMessages::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}
bool ConsumerFriendlyMessages::struct_empty() const {
  if (version.is_initialized()) {
    return false;
  }
  if (messages.is_initialized()) {
    return false;
  }

  return true;
}
void ConsumerFriendlyMessages::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (!version.is_valid()) {
    version.ReportErrors(&report__->ReportSubobject("version"));
  }
  if (!messages.is_valid()) {
    messages.ReportErrors(&report__->ReportSubobject("messages"));
  }
}

// ModuleMeta methods
ModuleMeta::ModuleMeta()
  : CompositeType(kUninitialized) {
}
ModuleMeta::~ModuleMeta() {
}
ModuleMeta::ModuleMeta(const Json::Value* value__)
  : CompositeType(InitHelper(value__, &Json::Value::isObject)) {
}
Json::Value ModuleMeta::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  return result__;
}
bool ModuleMeta::is_valid() const {
  if (struct_empty()) {
    return initialization_state__ == kInitialized;
  }
  return Validate();
}
bool ModuleMeta::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}
bool ModuleMeta::struct_empty() const {
  return true;
}
void ModuleMeta::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
}

// AppLevel methods
AppLevel::AppLevel()
  : CompositeType(kUninitialized) {
}
AppLevel::~AppLevel() {
}
AppLevel::AppLevel(const Json::Value* value__)
  : CompositeType(InitHelper(value__, &Json::Value::isObject)) {
}
Json::Value AppLevel::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  return result__;
}
bool AppLevel::is_valid() const {
  if (struct_empty()) {
    return initialization_state__ == kInitialized;
  }
  return Validate();
}
bool AppLevel::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}
bool AppLevel::struct_empty() const {
  return true;
}
void AppLevel::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
}

// UsageAndErrorCounts methods
UsageAndErrorCounts::UsageAndErrorCounts()
  : CompositeType(kUninitialized) {
}
UsageAndErrorCounts::~UsageAndErrorCounts() {
}
UsageAndErrorCounts::UsageAndErrorCounts(const Json::Value* value__)
  : CompositeType(InitHelper(value__, &Json::Value::isObject)),
    app_level(impl::ValueMember(value__, "app_level")) {
}
Json::Value UsageAndErrorCounts::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("app_level", app_level, &result__);
  return result__;
}
bool UsageAndErrorCounts::is_valid() const {
  if (struct_empty()) {
    return initialization_state__ == kInitialized;
  }
  if (!app_level.is_valid()) {
    return false;
  }
  return Validate();
}
bool UsageAndErrorCounts::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}
bool UsageAndErrorCounts::struct_empty() const {
  if (app_level.is_initialized()) {
    return false;
  }
  return true;
}
void UsageAndErrorCounts::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (!app_level.is_valid()) {
    app_level.ReportErrors(&report__->ReportSubobject("app_level"));
  }
}

// DeviceParams methods
DeviceParams::DeviceParams()
  : CompositeType(kUninitialized) {
}
DeviceParams::~DeviceParams() {
}
DeviceParams::DeviceParams(const Json::Value* value__)
  : CompositeType(InitHelper(value__, &Json::Value::isObject)) {
}
Json::Value DeviceParams::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  return result__;
}
bool DeviceParams::is_valid() const {
  if (struct_empty()) {
    return initialization_state__ == kInitialized;
  }
  return Validate();
}
bool DeviceParams::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}
bool DeviceParams::struct_empty() const {
  return true;
}
void DeviceParams::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
}

// PolicyTable methods
PolicyTable::PolicyTable()
  : CompositeType(kUninitialized) {
}
PolicyTable::PolicyTable(const ApplicationPolicies& app_policies, const FunctionalGroupings& functional_groupings, const ConsumerFriendlyMessages& consumer_friendly_messages, const ModuleConfig& module_config)
  : CompositeType(kUninitialized),
    app_policies(app_policies),
    functional_groupings(functional_groupings),
    consumer_friendly_messages(consumer_friendly_messages),
    module_config(module_config) {
}
PolicyTable::~PolicyTable() {
}
PolicyTable::PolicyTable(const Json::Value* value__)
  : CompositeType(InitHelper(value__, &Json::Value::isObject)),
    app_policies(impl::ValueMember(value__, "app_policies")),
    functional_groupings(impl::ValueMember(value__, "functional_groupings")),
    consumer_friendly_messages(impl::ValueMember(value__, "consumer_friendly_messages")),
    module_config(impl::ValueMember(value__, "module_config")),
    module_meta(impl::ValueMember(value__, "module_meta")),
    usage_and_error_counts(impl::ValueMember(value__, "usage_and_error_counts")),
    device_data(impl::ValueMember(value__, "device_data")) {
}
Json::Value PolicyTable::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("app_policies", app_policies, &result__);
  impl::WriteJsonField("functional_groupings", functional_groupings, &result__);
  impl::WriteJsonField("consumer_friendly_messages", consumer_friendly_messages, &result__);
  impl::WriteJsonField("module_config", module_config, &result__);
  impl::WriteJsonField("module_meta", module_meta, &result__);
  impl::WriteJsonField("usage_and_error_counts", usage_and_error_counts, &result__);
  impl::WriteJsonField("device_data", device_data, &result__);
  return result__;
}
bool PolicyTable::is_valid() const {
  if (!app_policies.is_valid()) {
    return false;
  }
  if (!functional_groupings.is_valid()) {
    return false;
  }
  if (!consumer_friendly_messages.is_valid()) {
    return false;
  }
  if (!module_config.is_valid()) {
    return false;
  }
  if (!module_meta.is_valid()) {
    return false;
  }
  if (!usage_and_error_counts.is_valid()) {
    return false;
  }
  if (!device_data.is_valid()) {
    return false;
  }
  return Validate();
}
bool PolicyTable::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}
bool PolicyTable::struct_empty() const {
  if (app_policies.is_initialized()) {
    return false;
  }
  if (functional_groupings.is_initialized()) {
    return false;
  }

  if (consumer_friendly_messages.is_initialized()) {
    return false;
  }
  if (module_config.is_initialized()) {
    return false;
  }

  if (module_meta.is_initialized()) {
    return false;
  }
  if (usage_and_error_counts.is_initialized()) {
    return false;
  }

  if (device_data.is_initialized()) {
    return false;
  }
  return true;
}
void PolicyTable::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (!app_policies.is_valid()) {
    app_policies.ReportErrors(&report__->ReportSubobject("app_policies"));
  }
  if (!functional_groupings.is_valid()) {
    functional_groupings.ReportErrors(&report__->ReportSubobject("functional_groupings"));
  }
  if (!consumer_friendly_messages.is_valid()) {
    consumer_friendly_messages.ReportErrors(&report__->ReportSubobject("consumer_friendly_messages"));
  }
  if (!module_config.is_valid()) {
    module_config.ReportErrors(&report__->ReportSubobject("module_config"));
  }
  if (!module_meta.is_valid()) {
    module_meta.ReportErrors(&report__->ReportSubobject("module_meta"));
  }
  if (!usage_and_error_counts.is_valid()) {
    usage_and_error_counts.ReportErrors(&report__->ReportSubobject("usage_and_error_counts"));
  }
  if (!device_data.is_valid()) {
    device_data.ReportErrors(&report__->ReportSubobject("device_data"));
  }
}

// Table methods
Table::Table()
  : CompositeType(kUninitialized) {
}
Table::Table(const PolicyTable& policy_table)
  : CompositeType(kUninitialized),
    policy_table(policy_table) {
}
Table::~Table() {
}
Table::Table(const Json::Value* value__)
  : CompositeType(InitHelper(value__, &Json::Value::isObject)),
    policy_table(impl::ValueMember(value__, "policy_table")) {
}
Json::Value Table::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("policy_table", policy_table, &result__);
  return result__;
}
bool Table::is_valid() const {
  if (!policy_table.is_valid()) {
    return false;
  }
  return Validate();
}
bool Table::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}
bool Table::struct_empty() const {
  if (policy_table.is_initialized()) {
    return false;
  }
  return true;
}
void Table::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (!policy_table.is_valid()) {
    policy_table.ReportErrors(&report__->ReportSubobject("policy_table"));
  }
}

}  // namespace policy_table_interface_base
}  // namespace rpc

