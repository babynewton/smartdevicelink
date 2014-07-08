/**
 * @file policy_table_schema.cc
 * @brief Policy table schema source file.
 */
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 'A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "policies/policy_table_schema.h"

#include <map>
#include <string>

#include "policies/policy_manager.h"
#include "smart_objects/object_optional_schema_item.h"
#include "smart_objects/array_schema_item.h"
#include "smart_objects/string_schema_item.h"
#include "smart_objects/enum_schema_item.h"


namespace policies {

using ::NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem;
using ::NsSmartDeviceLink::NsSmartObjects::CSmartSchema;
using ::NsSmartDeviceLink::NsSmartObjects::ISchemaItem;
using ::NsSmartDeviceLink::NsSmartObjects::CAlwaysTrueSchemaItem;
using ::NsSmartDeviceLink::NsSmartObjects::ObjectOptionalSchemaItem;
using ::NsSmartDeviceLink::NsSmartObjects::CArraySchemaItem;
using ::NsSmartDeviceLink::NsSmartObjects::CStringSchemaItem;
using ::NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem;
using ::NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter;

typedef utils::SharedPtr<ISchemaItem> SchemaItemPtr;

//-----------------------------------------------------------------------------

const char* PolicyTableSchema::kStrPolicyTable = "policy_table";
const char* PolicyTableSchema::kStrModuleConfig = "module_config";
const char* PolicyTableSchema::kStrFunctionalGroupings = "functional_groupings";
const char* PolicyTableSchema::kStrAppPolicies = "app_policies";
const char* PolicyTableSchema::kStrEndpoints = "endpoints";
const char* PolicyTableSchema::kStrDefault = "default";
const char* PolicyTableSchema::kStrUserConsentPrompt = "user_consent_prompt";
const char* PolicyTableSchema::kStrRpcs = "rpcs";
const char* PolicyTableSchema::kStrHmiLevels = "hmi_levels";
const char* PolicyTableSchema::kStrParameters = "parameters";
const char* PolicyTableSchema::kStrGroups = "groups";
const char* PolicyTableSchema::kStrNicknames = "nicknames";
const char* PolicyTableSchema::kStrPriority = "priority";

//-----------------------------------------------------------------------------

PolicyTableSchema::PolicyTableSchema(void)
  : schema_() {
}

//-----------------------------------------------------------------------------

CSmartSchema PolicyTableSchema::Create(void) {
  static PolicyTableSchema schema;

  return schema.GetSchema();
}

//-----------------------------------------------------------------------------

CSmartSchema PolicyTableSchema::GetSchema(void) {
  if (false == schema_.valid()) {
    schema_ = CreateSchema();
  }

  return CSmartSchema(schema_);
}

//-----------------------------------------------------------------------------

SchemaItemPtr PolicyTableSchema::CreateSchema(void) {
  SchemaItemPtr root_schema_item;
  std::map<std::string, CObjectSchemaItem::SMember> root_member_map;
  std::map<std::string, CObjectSchemaItem::SMember> policy_table_member_map;

  policy_table_member_map[kStrModuleConfig] = CObjectSchemaItem::SMember(
    CreateModuleConfig(),
    true);

  policy_table_member_map[kStrFunctionalGroupings] =
    CObjectSchemaItem::SMember(CreateFunctionalGroupings(), true);

  policy_table_member_map[kStrAppPolicies] = CObjectSchemaItem::SMember(
    CreateAppPolicies(),
    true);

  root_member_map[kStrPolicyTable] = CObjectSchemaItem::SMember(
    CObjectSchemaItem::create(policy_table_member_map),
    true);

  root_schema_item = CObjectSchemaItem::create(root_member_map);

  return root_schema_item;
}

//-----------------------------------------------------------------------------

SchemaItemPtr PolicyTableSchema::CreateModuleConfig(void) {
  std::map<std::string, CObjectSchemaItem::SMember> service_type_map;
  service_type_map[kStrDefault] = CObjectSchemaItem::SMember(
    CArraySchemaItem::create(CStringSchemaItem::create()), true);

  std::map<std::string, CObjectSchemaItem::SMember> endpoint_map;

  endpoint_map[ObjectOptionalSchemaItem::kOptionalGenericFieldName] =
    CObjectSchemaItem::SMember(
      CObjectSchemaItem::create(service_type_map), true);

  std::map<std::string, CObjectSchemaItem::SMember> module_config_map;
  module_config_map[kStrEndpoints] = CObjectSchemaItem::SMember(
    ObjectOptionalSchemaItem::create(endpoint_map), true);

  return CObjectSchemaItem::create(module_config_map);
}

//-----------------------------------------------------------------------------

SchemaItemPtr PolicyTableSchema::CreateFunctionalGroupings(void) {
  std::map<std::string, CObjectSchemaItem::SMember> rpc_map;

  rpc_map[kStrHmiLevels] = CObjectSchemaItem::SMember(
    CArraySchemaItem::create(CStringSchemaItem::create()) );

  rpc_map[kStrParameters] = CObjectSchemaItem::SMember(
    CArraySchemaItem::create(CStringSchemaItem::create()), false);

  std::map<std::string, CObjectSchemaItem::SMember> rpcs_map;
  rpcs_map[ObjectOptionalSchemaItem::kOptionalGenericFieldName] =
    CObjectSchemaItem::SMember(CObjectSchemaItem::create(rpc_map), false);

  std::map<std::string, CObjectSchemaItem::SMember> functional_grouping_map;

  functional_grouping_map[kStrUserConsentPrompt] =
    CObjectSchemaItem::SMember(CStringSchemaItem::create(), false);
  functional_grouping_map[kStrRpcs] = CObjectSchemaItem::SMember(
    ObjectOptionalSchemaItem::create(rpcs_map), true);

  std::map<std::string, CObjectSchemaItem::SMember> functional_groupings_map;

  functional_groupings_map[
    ObjectOptionalSchemaItem::kOptionalGenericFieldName] =
      CObjectSchemaItem::SMember(
        CObjectSchemaItem::create(functional_grouping_map), false);

  return ObjectOptionalSchemaItem::create(functional_groupings_map);
}

//-----------------------------------------------------------------------------

SchemaItemPtr PolicyTableSchema::CreatePriority(void) {
  std::set<Priority::eType> allowed_priorities;

  allowed_priorities.insert(Priority::PRIORITY_NONE);
  allowed_priorities.insert(Priority::PRIORITY_NORMAL);
  allowed_priorities.insert(Priority::PRIORITY_COMMUNICATION);
  allowed_priorities.insert(Priority::PRIORITY_NAVIGATION);
  allowed_priorities.insert(Priority::PRIORITY_EMERGENCY);

  SchemaItemPtr priority =
    TEnumSchemaItem<Priority::eType>::create(allowed_priorities);

  return priority;
}

//-----------------------------------------------------------------------------

SchemaItemPtr PolicyTableSchema::CreateAppId(void) {
  std::map<std::string, CObjectSchemaItem::SMember> app_id_map;

  app_id_map[kStrGroups] = CObjectSchemaItem::SMember(
    CArraySchemaItem::create(CStringSchemaItem::create()), true);
  app_id_map[kStrNicknames] = CObjectSchemaItem::SMember(
    CArraySchemaItem::create(CStringSchemaItem::create()), true);
  app_id_map[kStrPriority] = CObjectSchemaItem::SMember(
    CreatePriority(), true);

  return CObjectSchemaItem::create(app_id_map);
}

//-----------------------------------------------------------------------------

SchemaItemPtr PolicyTableSchema::CreateAppPoliciesDefault(void) {
  std::map<std::string, CObjectSchemaItem::SMember> default_map;

  default_map[kStrPriority] = CObjectSchemaItem::SMember(
    CreatePriority(), true);
  default_map[kStrGroups] = CObjectSchemaItem::SMember(
    CArraySchemaItem::create(CStringSchemaItem::create()), true);

  return CObjectSchemaItem::create(default_map);
}

//-----------------------------------------------------------------------------

SchemaItemPtr PolicyTableSchema::CreateAppPolicies(void) {
  std::map<std::string, CObjectSchemaItem::SMember> app_policies_map;

  app_policies_map[kStrDefault] =
    CObjectSchemaItem::SMember(CreateAppPoliciesDefault(), true);
  app_policies_map[ObjectOptionalSchemaItem::kOptionalGenericFieldName] =
    CObjectSchemaItem::SMember(CreateAppId(), true);

  return ObjectOptionalSchemaItem::create(app_policies_map);
}

}  // namespace policies

//-------------- String to value enum mapping ----------------
namespace NsSmartDeviceLink {
namespace NsSmartObjects {

template <>
const std::map<policies::Priority::eType, std::string>&
  TEnumSchemaItem<policies::Priority::eType>::
    getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<policies::Priority::eType, std::string>
    enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(
      std::make_pair(policies::Priority::PRIORITY_NONE, "NONE"));
    enum_string_representation.insert(
      std::make_pair(policies::Priority::PRIORITY_NORMAL, "NORMAL"));
    enum_string_representation.insert(
      std::make_pair(policies::Priority::PRIORITY_COMMUNICATION,
                     "COMMUNICATION"));
    enum_string_representation.insert(
      std::make_pair(policies::Priority::PRIORITY_NAVIGATION, "NAVIGATION"));
    enum_string_representation.insert(
      std::make_pair(policies::Priority::PRIORITY_EMERGENCY, "EMERGENCY"));

    is_initialized = true;
  }

  return enum_string_representation;
}

}  // namespace NsSmartObjects
}  // namespace NsSmartDeviceLink
