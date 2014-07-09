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

#ifndef SRC_COMPONENTS_POLICY_INCLUDE_POLICY_POLICY_MANAGER_H_
#define SRC_COMPONENTS_POLICY_INCLUDE_POLICY_POLICY_MANAGER_H_

#include <vector>

#include "policy/policy_types.h"
#include "policy/policy_listener.h"
#include "usage_statistics/statistics_manager.h"

namespace policy {

class PolicyManager : public usage_statistics::StatisticsManager {
 public:
  virtual ~PolicyManager() {
  }

  virtual void set_listener(PolicyListener* listener) = 0;

  /**
   *@brief Loads Policy Table from json file. Used for loading
   * Preloaded Policy Table or after Master Reset of the system.
   *@param file_name Path to preloaded PT file
   * @return bool Success of operation
   */
  virtual bool LoadPTFromFile(const std::string& file_name) = 0;

  /**
   * @brief Updates Policy Table from binary message received from
   * mobile device. Saves to Policy Table diff between Policy Table
   * sent in snapshot and received Policy Table.
   * @param pt_content PTU as binary string
   * @return bool Success of operation
   */
  virtual bool LoadPT(const BinaryMessage& pt_content) = 0;

  /**
   * @brief Gets URL for sending PTS to from PT itself.
     * @param service_type Service specifies user of URL
   * @return string URL
   */
    virtual std::string GetUpdateUrl(int service_type) = 0;

    /**
     * @brief Gets all URLs for sending PTS to from PT itself.
     * @param service_type Service specifies user of URL
     * @return vector of urls
     */
    virtual EndpointUrls GetUpdateUrls(int service_type) = 0;

  /**
   * @brief PTU is needed, for this PTS has to be formed and sent.
   * @return BinaryMessage* PTS.
   */
  virtual BinaryMessageSptr RequestPTUpdate() = 0;

  /**
   * @brief Check if specified RPC for specified application
   * has permission to be executed in specified HMI Level
   * and also its permitted params.
   * @param app_id Id of application provided during registration
   * @param hmi_level Current HMI Level of application
   * @param rpc Name of RPC
   * @return CheckPermissionResult containing flag if HMI Level is allowed
   * and list of allowed params.
   */
  virtual CheckPermissionResult CheckPermissions(const PTString& app_id,
                                                 const PTString& hmi_level,
                                                 const PTString& rpc) = 0;

  /**
   * @brief Clear all record of user consents. Used during Factory Reset.
   * @return bool Success of operation
   */
  virtual bool ResetUserConsent() = 0;

  /**
   * @brief Checks, if policy update is necessary for application
   * @param Application id assigned by Ford to the application
   */
  virtual void CheckAppPolicyState(const std::string& application_id) = 0;

  /**
   * @brief Returns current status of policy table for HMI
   * @return Current status of policy table
   */
  virtual PolicyTableStatus GetPolicyTableStatus() = 0;

  /**
   * Checks is PT exceeded IgnitionCycles
   * @return true if exceeded
   */
  virtual bool ExceededIgnitionCycles() = 0;

  /**
   * Checks is PT exceeded days
   * @param days current day after epoch
   * @return true if exceeded
   */
  virtual bool ExceededDays(int days) = 0;

  /**
   * Checks is PT exceeded kilometers
   * @param kilometers current kilometers at odometer
   * @return true if exceeded
   */
  virtual bool ExceededKilometers(int kilometers) = 0;

  /**
   * Increments counter of ignition cycles
   */
  virtual void IncrementIgnitionCycles() = 0;

  /**
   * Resets retry sequence
   */
  virtual void ResetRetrySequence() = 0;

  /**
   * Gets timeout to wait before next retry updating PT
   * If timeout is less or equal to zero then the retry sequence is not need.
   * @return timeout in seconds
   */
  virtual int NextRetryTimeout() = 0;

  /**
   * Gets timeout to wait until receive response
   * @return timeout in seconds
   */
  virtual int TimeoutExchange() = 0;

  /**
   * @brief List of timeouts in seconds between retries
   *        when attempt to update PT fails
   * @return List of delays between attempts.
   */
  virtual const std::vector<int> RetrySequenceDelaysSeconds() = 0;

  /**
   * Handler of exceeding timeout of exchanging policy table
   */
  virtual void OnExceededTimeout() = 0;

  /**
   * @brief Check user consent for mobile device data connection
   * @param device_id Unique device identifier
   * @return status of device consent
   */
  virtual DeviceConsent GetUserConsentForDevice(
      const std::string& device_id) = 0;

  /**
   * @brief Set user consent for mobile device data connection
   * @param device_id Unique device identifier
   * @param is_allowed User consent for usage device data connection
   */
  virtual void SetUserConsentForDevice(const std::string& device_id,
                                       bool is_allowed) = 0;
  /**
   * Sets number of kilometers and days after epoch, that passed for
   * receiving PT UPdate.
   */
  virtual void PTUpdatedAt(int kilometers, int days_after_epoch) = 0;

  /**
   * @brief Retrieves data from app_policies about app on its registration:
   * @param app_id - id of registered app
   * @param app_types Section on HMI where app can appear (Navigation, Phone etc)
   * @param nicknames Synonyms for application
   */
  virtual bool GetInitialAppData(const std::string& application_id,
                                 StringArray* nicknames = NULL,
                                 StringArray* app_hmi_types = NULL) = 0;
  /**
   * @brief Stores device parameters received during application registration
   * to policy table
   * @param device_id Device mac address
   * @param device_info Received device parameters
   */
  virtual void SetDeviceInfo(const std::string& device_id,
                             const DeviceInfo& device_info) = 0;

  /**
   * @brief Set user consent for application functional groups
   * @param permissions User-defined application group pemissions
   */
  virtual void SetUserConsentForApp(const PermissionConsent& permissions) = 0;

  /**
   * @brief Get default HMI level for application
   * @param policy_app_id Unique application id
   * @param default_hmi Default HMI level for application or empty, if value
   * was not set
   * @return true, if succedeed, otherwise - false
   */
  virtual bool GetDefaultHmi(const std::string& policy_app_id,
                             std::string* default_hmi) = 0;

  /**
   * @brief Get priority for application
   * @param policy_app_id Unique application id
   * @param priority Priority for application or empty, if value was not set
   * @return true, if succedeed, otherwise - false
   */
  virtual bool GetPriority(const std::string& policy_app_id,
                           std::string* priority) = 0;

  /**
   * @brief Get user friendly messages for given RPC messages and language
   * @param message_codes RPC message codes
   * @param language Language
   * @return Array of structs with appropriate message parameters
   */
  virtual std::vector<UserFriendlyMessage> GetUserFriendlyMessages(
      const std::vector<std::string>& message_code,
      const std::string& language) = 0;

  /**
   * Checks if the application is revoked
   * @param app_id application id
   * @return true if application is revoked
   */
  virtual bool IsApplicationRevoked(const std::string& app_id) const = 0;

  /**
   * @brief Get user permissions for application which started on specific device
   * @param device_id Device id
   * @param policy_app_id Unique application id
   * @param permissions Array of functional groups permissions
   */
  virtual void GetUserPermissionsForApp(
      const std::string& device_id, const std::string& policy_app_id,
      std::vector<FunctionalGroupPermission>& permissions) = 0;
  virtual AppPermissions GetAppPermissionsChanges(
      const std::string& app_id) = 0;
  virtual void RemovePendingPermissionChanges(const std::string& app_id) = 0;

  /**
   * @brief Update current device id, which data should be proceeded
   * @param device_id Current device id
   */
  virtual void UpdateCurrentDeviceId(const std::string& device_id) = 0;

  /**
   * @brief Return device id, which hosts specific application
   * @param Application id, which is required to update device id
   */
  virtual std::string& GetCurrentDeviceId(const std::string& policy_app_id) = 0;

  /**
   * @brief Set current system language
   * @param language Language
   */
  virtual void SetSystemLanguage(const std::string& language) = 0;

  /**
   * @brief Set data from GetSystemInfo response to policy table
   * @param ccpu_version CCPU version
   * @param wers_country_code WERS country code
   * @param language System language
   */
  virtual void SetSystemInfo(const std::string& ccpu_version,
                             const std::string& wers_country_code,
                             const std::string& language) = 0;
};

}  // namespace policy

extern "C" policy::PolicyManager* CreateManager();

#endif  // SRC_COMPONENTS_POLICY_INCLUDE_POLICY_POLICY_MANAGER_H_
