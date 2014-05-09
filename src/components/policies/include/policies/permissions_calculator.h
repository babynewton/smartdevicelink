/**
 * @file permissions_calculator.h
 * @brief Permissions calculator header file.
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

#ifndef SRC_COMPONENTS_POLICIES_INCLUDE_POLICIES_PERMISSIONS_CALCULATOR_H_
#define SRC_COMPONENTS_POLICIES_INCLUDE_POLICIES_PERMISSIONS_CALCULATOR_H_

#include <vector>
#include <string>
#include "policies/policy_manager.h"
#include "interfaces/MOBILE_API.h"
#include "utils/logger.h"

namespace policies {

class PermissionsCalculator {
  public:
    /**
      * @brief Calculate permissions for application.
      *
      * @param pt_object Policy Table as smart object
      * @param app_id  Application identifier
      * @param rpc     Remote procedure call
      * @param hmi_status  HMI level status
      *
      * @return result of check permission
      */
    static PermissionResult CalcPermissions(
        const NsSmartDeviceLink::NsSmartObjects::SmartObject& pt_object,
        uint32_t app_id,
        const NsSmartDeviceLink::NsSmartObjects::SmartObject& rpc,
        mobile_apis::HMILevel::eType hmi_status);

    /**
    * @brief Get priority for app_id
    *
    * @param pt_object Policy Table as smart object
    * @param app_id Application Id
    *
    * @return priority for app_id
    */
    static Priority::eType GetPriority(
        const NsSmartDeviceLink::NsSmartObjects::SmartObject& pt_object,
        uint32_t app_id);

  private:
    /**
     * @brief Find all RPCs in groups listed under section functional_groupings
     *
     * @param pt_object   Policy Table as smart object
     * @param rpc_groups  Groups of RPCs
     * @param rpc         Remote procedure call
     * @param hmi_status  HMI level status
     *
     * @return  Map of rpc to all possible HMI levels amongst all
     *          functional groups where RPC is found.
     *          Returned value must be freed by the caller.
     */
    static PermissionResult CalcPermissionsByGroups(
        const NsSmartDeviceLink::NsSmartObjects::SmartObject& pt_object,
        const std::vector<std::string>& rpc_groups,
        const NsSmartDeviceLink::NsSmartObjects::SmartObject& rpc,
        mobile_apis::HMILevel::eType hmi_status);

    /**
    * @brief Returns a smart object containing an <app_id> section of PolicyTable
    *
    * If app_id is not found in the "app_policies" the "default" section is
    * returned.
    * @param pt_object  Policy Table as a SmartObject
    * @param app_id     Application ID
    *
    * @return SmartObject that contains either <app_id> or "default" section
    *         of "app_policies".
    */
    static const NsSmartDeviceLink::NsSmartObjects::SmartObject&
      GetPolicyTableAppIdSection(
        const NsSmartDeviceLink::NsSmartObjects::SmartObject& pt_object,
        uint32_t app_id);

    /**
     * @brief Convert HMI level value (as number) to string
     *
     * @param hmi_status        HMI level status
     * @param hmi_level_string  Pointer to a string where the result is
     *  retunred.
     */
    static void ConvertHMILevel2String(
        mobile_apis::HMILevel::eType hmi_status,
        std::string* hmi_level_string);

    /**
     * @breif Convert smart object to vector of strings
     *
     * @param object Smart object
     * @param v_strings Pointer to a verctor of strings where result is
     *  returned.
     */
    static void ConvertSmartArray2VectorStrings(
        const NsSmartDeviceLink::NsSmartObjects::SmartObject& object,
        std::vector<std::string>* v_strings);
    /**
     * @brief Logger
     */
    static log4cxx::LoggerPtr logger_;
};

}  // namespace policies

#endif  // SRC_COMPONENTS_POLICIES_INCLUDE_POLICIES_PERMISSIONS_CALCULATOR_H_
