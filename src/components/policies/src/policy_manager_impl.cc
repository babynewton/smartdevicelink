/**
 * @file policy_manager_impl.cc
 * @brief Policy Manager implementation source file.
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

#include "policies/policy_manager_impl.h"
#include "policies/permissions_calculator.h"
#include "utils/file_system.h"
#include "utils/macro.h"

namespace policies {

namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;

#ifdef ENABLE_LOG
log4cxx::LoggerPtr PolicyManagerImpl::logger_ = log4cxx::LoggerPtr(
    log4cxx::Logger::getLogger("Policies"));
#endif // ENABLE_LOG

//---------------------------------------------------------------

PolicyManagerImpl::PolicyManagerImpl()
  : PolicyManager()
  , policy_config_()
  , policy_table_(NULL)
  , init_result_(InitResult::INIT_FAILED) {
}

//---------------------------------------------------------------

PolicyManagerImpl::~PolicyManagerImpl() {
  if (NULL != policy_table_) {
    delete policy_table_;
    policy_table_ = NULL;
  }
}

//---------------------------------------------------------------

InitResult PolicyManagerImpl::Init(
  const PolicyConfiguration& config) {
  // TODO(anyone): Provide some mechanism for recovery (from Preload???)
  // if PT file corrupted (e.g. bad json)
  policy_config_ = config;

  std::string pt_string;
  if (NULL == policy_table_) {
    if (true == file_system::ReadFile(policy_config_.pt_file_name(),
                                      pt_string)) {
      policy_table_ = new PolicyTable(pt_string,
                                      PolicyTableType::TYPE_POLICY_TABLE);
      init_result_ = InitResult::INIT_OK;
    } else {
      LOG4CXX_WARN(logger_, "Can't read policy table file "
          << policy_config_.pt_file_name());
      if (true == file_system::ReadFile(
          policy_config_.preload_pt_file_name(), pt_string)) {
        policy_table_ = new PolicyTable(
            pt_string, PolicyTableType::TYPE_PT_PRELOAD);
        init_result_ = InitResult::INIT_OK;
      } else {
        init_result_ = InitResult::INIT_FAILED;
        LOG4CXX_ERROR(logger_, "Can't read Preload policy table file "
            << policy_config_.preload_pt_file_name());
      }
    }
  } else {
    init_result_ = InitResult::INIT_OK;
    LOG4CXX_WARN(logger_, "Policy table is already created.");
  }
  return init_result_;
}

//---------------------------------------------------------------

CheckPermissionResult
  PolicyManagerImpl::CheckPermission(
    uint32_t app_id,
    const smart_objects::SmartObject& rpc,
    mobile_apis::HMILevel::eType hmi_status) {
//
  CheckPermissionResult result =
    {PermissionResult::PERMISSION_DISALLOWED, Priority::PRIORITY_NONE};
  PolicyTable* pt = policy_table();

  if (init_result_ == InitResult::INIT_FAILED) {
    result.result = PermissionResult::PERMISSION_INIT_FAILED;
  } else if (NULL != pt
            && PTValidationResult::VALIDATION_OK == pt->Validate()) {
    smart_objects::SmartObject& pt_object = pt->AsSmartObject();

    result.result =  PermissionsCalculator::CalcPermissions(pt_object,
                                                            app_id,
                                                            rpc,
                                                            hmi_status);

    result.priority = PermissionsCalculator::GetPriority(pt_object, app_id);
  } else {
    result.result = PermissionResult::PERMISSION_PT_VERIFICATION_FAILED;
  }

  return result;
}

//---------------------------------------------------------------

bool PolicyManagerImpl::StorePolicyTable() {
  if (NULL == policy_table_) {
    LOG4CXX_WARN(logger_, "PolicyTable is NULL."
                          "It seems like Init hasn't been called");
    NOTREACHED();
    return false;
  }

  if (smart_objects::SmartType_Map !=
        policy_table_->AsSmartObject().getType()) {
    LOG4CXX_WARN(logger_, "PolicyTable is empty. Nothing to store");
    return false;
  }

  std::string pt_string;
  if (false == policy_table_->AsString(&pt_string)) {
    LOG4CXX_ERROR(logger_, "Can't convert PolicyTable to string.");
    return false;
  }

  const std::vector<uint8_t> char_vector_pdata(
    pt_string.begin(), pt_string.end());

  if (false == file_system::Write(policy_config_.pt_file_name(),
                                  char_vector_pdata)) {
    LOG4CXX_ERROR(logger_, "Can't write policy table file "
      << policy_config_.pt_file_name());
    return false;
  }
  return true;
}

//---------------------------------------------------------------

PolicyTable* PolicyManagerImpl::policy_table() const {
  if (NULL == policy_table_) {
    LOG4CXX_ERROR(logger_, "Accessing not initialized policy table.");
  }
  return policy_table_;
}

}  // namespace policies
