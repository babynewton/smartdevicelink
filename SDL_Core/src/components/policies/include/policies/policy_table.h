/**
 * @file policy_table.h
 * @brief Policy table header file.
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

#ifndef SRC_COMPONENTS_POLICIES_INCLUDE_POLICIES_POLICY_TABLE_H_
#define SRC_COMPONENTS_POLICIES_INCLUDE_POLICIES_POLICY_TABLE_H_

#include <string>
#include "smart_objects/smart_object.h"
#include "smart_objects/smart_schema.h"

namespace policies {

enum PTValidationResult {
  VALIDATION_OK = 0,
  VALIDATION_FAILED_BAD_JSON,
  VALIDATION_FAILED
};

enum PolicyTableType {
  TYPE_POLICY_TABLE = 0,
  TYPE_PT_PRELOAD
};

class PolicyTable {
  public:
    /**
     * @brief Constructor
     * 
     * @param policy_table_string String containing policy table 
     * @param pt_type PolicyTable type (PT or Preload)
     **/
    PolicyTable(const std::string& policy_table_string,
                PolicyTableType pt_type);

    /**
     * @brief Destructor
     */
    ~PolicyTable();

    /**
     * @brief Get policy table as smart object
     *
     * @return Policy table as smart object
     */
    NsSmartDeviceLink::NsSmartObjects::SmartObject& AsSmartObject();

    /**
     * @brief Get policy table as a string
     *
     * @param policy_table_string pointer to the string where PolicyTable
     *  will be written.
     * 
     * @return true if PolicyTable correctly converted to string and the string
     *  is returned in "policy_table_string"
     */
    bool AsString(std::string* policy_table_string) const;

    /**
    * @brief Validate policy table.
    *
    * Validates policy table against smart schema which should be set before.
    *
    * @return validation result as enum
    **/
    PTValidationResult Validate();

    /**
    * @brief Tells wether Policy Table is Preload
    *
    * @return whether Policy Table Preload
    */
    bool IsPTPreload();

  private:
    /**
     * @brief is Policy Table valid
     **/
    PTValidationResult pt_validation_result_;

    /**
     * @brief PolicyTable type
     **/
    PolicyTableType pt_type_;

    /**
     * @brief Schema to verify policy table
     */
    NsSmartDeviceLink::NsSmartObjects::CSmartSchema schema_;

    /**
     * @brief policy table as smart object
     */
    NsSmartDeviceLink::NsSmartObjects::SmartObject pt_smart_object_;
};

}  // namespace policies

#endif  // SRC_COMPONENTS_POLICIES_INCLUDE_POLICIES_POLICY_TABLE_H_
