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

#ifndef VALIDATED_TYPES_H_
#define VALIDATED_TYPES_H_

#include <stdint.h>
#include <map>
#include <string>
#include <vector>

namespace Json {
class Value;
}  // namespace Json

namespace dbus {
class MessageReader;
class MessageWriter;
}  // namespace dbus

namespace rpc {
template<typename T> class Range;
class PrimitiveType;
class CompositeType;
class Boolean;
template<typename T, T minval, T maxval> class Integer;
template<int64_t minnum, int64_t maxnum,
         int64_t minden = 1, int64_t maxden = 1> class Float;
template<size_t minlen, size_t maxlen> class String;
template<typename T> class Enum;
template<typename T, size_t minsize, size_t maxsize> class Array;
template<typename T, size_t minsize, size_t maxsize> class Map;
template<typename T> class Nullable;
template<typename T> class Mandatory;
template<typename T> class Optional;

template<typename T>
class Range {
 public:
  // Methods
  Range(const T min, const T max);
  T min() const;
  T max() const;
  template<typename U>
  bool Includes(U val) const;
 private:
  T min_;
  T max_;
};

/*
 * Base class for all primitive types, keeps a flag to
 * tell whether descendant object was initialized
 */
class PrimitiveType {
 public:
  bool is_initialized() const;
  bool is_valid() const;
 protected:
  enum ValueState {
    kUninitialized,
    kInvalid,
    kValid
  };
  PrimitiveType(ValueState value_state);
  static ValueState InitHelper(const Json::Value* value,
                               bool (Json::Value::*type_check)() const);
 protected:
  ValueState value_state_;
};

/*
 * Helper class for all composite types (arrays and all user-defined types)
 */
class CompositeType {
 public:
  static const Json::Value* ValueMember(const Json::Value* value,
                                        const char* member_name);
  template<class T, size_t minsize, size_t maxsize>
  static void WriteJsonField(const char* field_name,
                             const Array<T, minsize, maxsize>& field,
                             Json::Value* json_value);
  template<class T, size_t minsize, size_t maxsize>
  static void WriteJsonField(const char* field_name,
                             const Map<T, minsize, maxsize>& field,
                             Json::Value* json_value);
  template<class T>
  static void WriteJsonField(const char* field_name,
                             const Nullable<T>& field,
                             Json::Value* json_value);
  template<class T>
  static void WriteJsonField(const char* field_name, const Optional<T>& field,
                             Json::Value* json_value);
  template<class T>
  static void WriteJsonField(const char* field_name, const Mandatory<T>& field,
                             Json::Value* json_value);
};

/*
 * Class that holds primitive boolean value. It is always valid.
 */
class Boolean : public PrimitiveType {
 public:
  // Types
  typedef bool ValueType;
 public:
  // Methods
  Boolean();
  explicit Boolean(bool value);
  explicit Boolean(const Json::Value* value);
  explicit Boolean(dbus::MessageReader* reader);
  Boolean(const Json::Value* value, bool def_value);
  Boolean& operator=(bool new_val);
  operator bool() const;
  Json::Value ToJsonValue() const;
  void ToDbusWriter(dbus::MessageWriter* writer) const;

private:
  // Fields
  ValueType value_;
};

template<typename T, T minval, T maxval>
class Integer : public PrimitiveType {
 public:
  // Types
  typedef T IntType;
 public:
  // Methods
  Integer();
  explicit Integer(IntType value);
  explicit Integer(const Json::Value* value);
  explicit Integer(dbus::MessageReader* reader);
  Integer(const Json::Value* value, IntType def_value);
  Integer& operator=(IntType new_val);
  operator IntType() const;
  Json::Value ToJsonValue() const;
  void ToDbusWriter(dbus::MessageWriter* writer) const;

 private:
  IntType value_;
  static const Range<T> range_;
};

template<int64_t minnum, int64_t maxnum, int64_t minden, int64_t maxden>
class Float : public PrimitiveType {
 public:
  // Methods
  Float();
  explicit Float(double value);
  explicit Float(const Json::Value* value);
  explicit Float(dbus::MessageReader* reader);
  Float(const Json::Value* value, double def_value);
  Float& operator=(double new_val);
  operator double() const;
  Json::Value ToJsonValue() const;
  void ToDbusWriter(dbus::MessageWriter* writer) const;

 private:
  double value_;
  static const Range<double> range_;
};

template<size_t minlen, size_t maxlen>
class String : public PrimitiveType {
 public:
  // Methods
  String();
  explicit String(const std::string& value);
  explicit String(const char* value);
  explicit String(const Json::Value* value);
  explicit String(dbus::MessageReader* reader);
  String(const Json::Value* value, const std::string& def_value);
  String& operator=(const std::string& new_val);
  operator const std::string&() const;
  Json::Value ToJsonValue() const;
  void ToDbusWriter(dbus::MessageWriter* writer) const;

 private:
  std::string value_;
  static const Range<size_t> length_range_;
};

template<typename T>
class Enum : public PrimitiveType {
 public:
  // Types
  typedef T EnumType;
 public:
  // Methods
  Enum();
  explicit Enum(EnumType value);
  explicit Enum(const Json::Value* value);
  explicit Enum(dbus::MessageReader* reader);
  Enum(const Json::Value* value, EnumType def_value);
  Enum& operator=(EnumType new_val);
  operator EnumType() const;
  Json::Value ToJsonValue() const;
  void ToDbusWriter(dbus::MessageWriter* writer) const;

 private:
  // Fields
  EnumType value_;
};

template<typename T, size_t minsize, size_t maxsize>
class Array : public std::vector<T> {
 public:
  // Types
  typedef std::vector<T> ArrayType;
 public:
  // Methods
  Array();
  // Need const and non-const versions to beat all-type accepting constructor
  explicit Array(Json::Value* value);
  explicit Array(const Json::Value* value);
  explicit Array(dbus::MessageReader* reader);
  template<typename U>
  explicit Array(const U& value);
  template<typename U>
  Array& operator=(const U& that);
  using ArrayType::push_back;
  template<typename U>
  void push_back(const U& value);
  Json::Value ToJsonValue() const;
  void ToDbusWriter(dbus::MessageWriter* writer) const;

  bool is_valid() const;
  bool is_initialized() const;
  bool is_null() const;
  void set_to_null();
 private:
  bool marked_as_null_;
};

template<typename T, size_t minsize, size_t maxsize>
class Map : public std::map<std::string, T> {
 public:
  // Types
  typedef std::map<std::string, T> MapType;
 public:
  // Methods
  Map();
  // Need const and non-const versions to beat all-type accepting constructor
  explicit Map(Json::Value* value);
  explicit Map(const Json::Value* value);
  explicit Map(dbus::MessageReader* reader);
  template<typename U>
  explicit Map(const U& value);
  template<typename U>
  Map& operator=(const U& that);
  using MapType::insert;
  template<typename U>
  void insert(const std::pair<std::string, U>& value);
  Json::Value ToJsonValue() const;
  void ToDbusWriter(dbus::MessageWriter* writer) const;

  bool is_valid() const;
  bool is_initialized() const;
  bool is_null() const;
  void set_to_null();
 private:
  bool marked_as_null_;
};

template<typename T>
class Nullable : public T {
 public:
  // Methods
  Nullable();
  // Need const and non-const versions to beat all-type accepting constructor
  explicit Nullable(Json::Value* value);
  explicit Nullable(const Json::Value* value);
  template<typename U>
  explicit Nullable(const U& value);
  template<typename U>
  Nullable(const Json::Value* value, const U& def_value);
  template<typename U>
  Nullable& operator=(const U& new_val);
  Json::Value ToJsonValue() const;

  bool is_valid() const;
  bool is_initialized() const;
  bool is_null() const;
  void set_to_null();
 private:
  bool marked_null_;
};

template<typename T>
class Mandatory : public T {
 public:
  // Methods
  Mandatory();
  template<typename U>
  explicit Mandatory(const U& value);
  template<typename U>
  Mandatory(const Json::Value* value, const U& def_value);
  template<typename U>
  Mandatory& operator=(const U& new_val);

  bool is_valid() const;
};

template<typename T>
class Optional {
 public:
  // Methods
  Optional();
  explicit Optional(dbus::MessageReader* reader);
  template<typename U>
  explicit Optional(const U& value);
  template<typename U>
  Optional(const Json::Value* value,const U& def_value);

  void ToDbusWriter(dbus::MessageWriter* writer) const;

  // Pointer semantics
  T& operator*();
  const T& operator*() const;
  T* operator->();
  const T* operator->() const;
  // For pointer-like 'if (optional_value)' tests
  // Better than operator bool because bool can be implicitly
  // casted to integral types
  operator const void*() const;

  bool is_valid() const;
  bool is_initialized() const;
 private:
  T value_;
};

}  // namespace rpc

// Template methods implementation
#include "rpc_base_inl.h"

#endif /* VALIDATED_TYPES_H_ */
