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

#ifndef SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_ENUM_SCHEMA_ITEM_H_
#define SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_ENUM_SCHEMA_ITEM_H_

#include <map>
#include <set>
#include <string>

#include "utils/shared_ptr.h"

#include "smart_objects/smart_object.h"
#include "smart_objects/schema_item.h"
#include "smart_objects/schema_item_parameter.h"


namespace NsSmartDeviceLink {
namespace NsSmartObjects {
/**
 * @brief Enumeration schema item.
 *
 * @tparam EnumType Enumeration type.
 **/
template<typename EnumType>
class TEnumSchemaItem : public ISchemaItem {
 public:
  /**
   * @brief Create a new schema item.
   *
   * @param AllowedElements Set of allowed enumeration elements.
   * @param DefaultValue Default value.
   *
   * @return Shared pointer to a new schema item.
   **/
  static utils::SharedPtr<TEnumSchemaItem> create(
      const std::set<EnumType>& AllowedElements,
      const TSchemaItemParameter<EnumType> & DefaultValue =
          TSchemaItemParameter<EnumType>());

  /**
   * @brief Validate smart object.
   *
   * @param Object Object to validate.
   *
   * @return NsSmartObjects::Errors::eType
   **/
  virtual Errors::eType validate(const SmartObject& Object);

  /**
   * @brief Set default value to an object.
   *
   * @param Object Object to set default value.
   *
   * @return true if default value was successfully set, false otherwise.
   **/
  // TODO(cpplint): Is this a non-const reference?
  // If so, make const or use a pointer.
  virtual bool setDefaultValue(SmartObject& Object);

  /**
   * @brief Apply schema.
   *
   * This implementation checks if enumeration is represented as string
   * and tries to convert it to integer according to element-to-string
   * map provided by getEnumElementsStringRepresentation().
   *
   * @param Object Object to apply schema.
   **/
  // TODO(cpplint): Is this a non-const reference?
    // If so, make const or use a pointer.
  virtual void applySchema(SmartObject& Object);

  /**
   * @brief Unapply schema.
   *
   * @param Object Object to unapply schema.
   **/
  // TODO(cpplint): Is this a non-const reference?
    // If so, make const or use a pointer.
  virtual void unapplySchema(SmartObject& Object);

  /**
   * @brief Build smart object by smart schema having copied matched
   *        parameters from pattern smart object
   *
   * @param pattern_object pattern object
   * @param result_object object to build
   */
  virtual void BuildObjectBySchema(const SmartObject& pattern_object,
                                   SmartObject& result_object);

  /**
   * @brief The method converts a string into the value of enum EnumType
   *
   * @param str String to convert
   * @param value the resulting enum value
   * @return true if the string is converted successfully
   */
  static bool stringToEnum(const std::string& str, EnumType &value);

  /**
   * @brief Get string representation of enumeration elements.
   *
   * @return Map of enum element to its string representation.
   **/
  static const std::map<EnumType, std::string>&
    getEnumElementsStringRepresentation();

  virtual ~TEnumSchemaItem() {
  }

 private:
  /**
   * @brief Constructor.
   *
   * @param AllowedElements Set of allowed enumeration elements.
   * @param DefaultValue Default value.
   **/
  TEnumSchemaItem(const std::set<EnumType>& AllowedElements,
                  const TSchemaItemParameter<EnumType>& DefaultValue);

  /**
   * @brief Copy constructor.
   *
   * Not implemented to prevent misuse.
   *
   * @param Other Other schema item.
   **/
  TEnumSchemaItem(const TEnumSchemaItem<EnumType>& Other);

  /**
   * @brief Assignment operator.
   *
   * Not implemented to prevent misuse.
   *
   * @param Other Other schema item.
   *
   * @return Not implemented.
   **/
  TEnumSchemaItem & operator =(const TEnumSchemaItem<EnumType>& Other);

  /**
   * @brief Set of allowed enumeration elements.
   **/
  const std::set<EnumType> mAllowedElements;

  /**
   * @brief Default value.
   **/
  const TSchemaItemParameter<EnumType> mDefaultValue;
};

template<typename EnumType>
utils::SharedPtr<TEnumSchemaItem<EnumType> > TEnumSchemaItem<EnumType>::create(
    const std::set<EnumType>& AllowedElements,
    const TSchemaItemParameter<EnumType>& DefaultValue) {
  return new TEnumSchemaItem<EnumType>(AllowedElements, DefaultValue);
}

template<typename EnumType>
Errors::eType TEnumSchemaItem<EnumType>::validate(const SmartObject& Object) {
  Errors::eType result = Errors::ERROR;

  if (SmartType_Integer == Object.getType()) {
    if (mAllowedElements.end()
        != mAllowedElements.find(static_cast<EnumType>(Object.asInt()))) {
      result = Errors::OK;
    } else {
      result = Errors::OUT_OF_RANGE;
    }
  } else {
    result = Errors::INVALID_VALUE;
  }

  return result;
}

// TODO(cpplint): Is this a non-const reference?
// If so, make const or use a pointer.
template<typename EnumType>
bool TEnumSchemaItem<EnumType>::setDefaultValue(SmartObject& Object) {
  bool result = false;
  EnumType value;

  if (true == mDefaultValue.getValue(value)) {
    Object = static_cast<int32_t>(value);
    result = true;
  }

  return result;
}

// TODO(cpplint): Is this a non-const reference?
// If so, make const or use a pointer.
template<typename EnumType>
void TEnumSchemaItem<EnumType>::applySchema(SmartObject& Object) {
  if (SmartType_String == Object.getType()) {
    std::string stringValue = Object.asString();
    const std::map<EnumType, std::string> elementsStringRepresentation =
        getEnumElementsStringRepresentation();

    for (typename std::map<EnumType, std::string>::const_iterator i =
        elementsStringRepresentation.begin();
        i != elementsStringRepresentation.end(); ++i) {
      if (i->second == stringValue) {
        Object = static_cast<int32_t>(i->first);
        break;
      }
    }
  }
}

// TODO(cpplint): Is this a non-const reference?
// If so, make const or use a pointer.
template<typename EnumType>
void TEnumSchemaItem<EnumType>::unapplySchema(SmartObject& Object) {
  if (SmartType_Integer == Object.getType()) {
    int32_t integerValue = Object.asInt();
    const std::map<EnumType, std::string> elementsStringRepresentation =
        getEnumElementsStringRepresentation();
    typename std::map<EnumType, std::string>::const_iterator i =
        elementsStringRepresentation.find(static_cast<EnumType>(integerValue));

    if (i != elementsStringRepresentation.end()) {
      Object = i->second;
    }
  }
}

template<typename EnumType>
void TEnumSchemaItem<EnumType>::BuildObjectBySchema(
    const SmartObject& pattern_object, SmartObject& result_object) {

  if (SmartType_Integer == pattern_object.getType()) {
    result_object = pattern_object;  // TODO(AK): Check this...
  } else {
    bool result = setDefaultValue(result_object);
    if (false == result) {
      result_object = static_cast<EnumType>(EnumType::INVALID_ENUM);
    }
  }
}

template<typename EnumType>
TEnumSchemaItem<EnumType>::TEnumSchemaItem(
    const std::set<EnumType> & AllowedElements,
    const TSchemaItemParameter<EnumType> & DefaultValue)
    : mAllowedElements(AllowedElements),
      mDefaultValue(DefaultValue) {
}

template<typename EnumType>
bool TEnumSchemaItem<EnumType>::stringToEnum(const std::string& str,
                                             EnumType& value) {
  bool result = false;
  std::map<EnumType, std::string> enumMap =
      TEnumSchemaItem<EnumType>::getEnumElementsStringRepresentation();

  for (typename std::map<EnumType, std::string>::const_iterator it = enumMap
      .begin(); it != enumMap.end(); ++it) {
    if (str == it->second) {
      value = it->first;
      result = true;
      break;
    }
  }

  return result;
}

}  // namespace NsSmartObjects
}  // namespace NsSmartDeviceLink

#endif  // SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_ENUM_SCHEMA_ITEM_H_
