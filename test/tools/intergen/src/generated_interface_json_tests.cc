#include "gmock/gmock.h"

#include <test_rpc_interface/interface.h>
#include <test_rpc_interface/functions.h>
#include "json/reader.h"
#include "json/writer.h"

namespace test {
using namespace rpc::test_rpc_interface;
using Json::Value;

class TestRequestHandlerMock: public request::Handler {
public:
  MOCK_METHOD1(HandleAddSubMenu, void(const request::AddSubMenu& params));
  MOCK_METHOD1(HandleDiagnosticMessage, void(const request::DiagnosticMessage& params));
  ~TestRequestHandlerMock() {}
};

class GeneratedInterfaceTests: public ::testing::Test {
 public:
  Json::Value JsonValue(const char* json) {
    Json::Value value;
    reader.parse(json, value);
    return value;
  }
  Json::Reader reader;
  Json::FastWriter writer;
};

TEST_F(GeneratedInterfaceTests, ScrollableMessageTest) {
  const char* org_json = "{\"reason\":\"MASTER_RESET\"}\n";
  Value json_value = JsonValue(org_json);
  notification::OnAppInterfaceUnregistered oaiu(&json_value);
  ASSERT_TRUE(oaiu.is_initialized());
  ASSERT_TRUE(oaiu.is_valid());

  std::string serialized = writer.write(oaiu.ToJsonValue());
  ASSERT_EQ(org_json, serialized);
}

TEST_F(GeneratedInterfaceTests, FunctionWithoutParams) {
  notification::OnAudioPassThru oapt;
  ASSERT_FALSE(oapt.is_initialized());
  ASSERT_TRUE(oapt.is_valid());
  std::string serialized = writer.write(oapt.ToJsonValue());
  ASSERT_EQ("{}\n", serialized);
}

TEST_F(GeneratedInterfaceTests, DefValueTest) {
  const char* org_json = "{\"menuID\":2,\"menuName\":\"Hello\"}";
  const char* awaited_json = "{\"menuID\":2,\"menuName\":\"Hello\",\"position\":1000}\n";
  Value json_value = JsonValue(org_json);
  request::AddSubMenu aasm(&json_value);
  ASSERT_TRUE(aasm.is_initialized());
  ASSERT_TRUE(aasm.is_valid());
  ASSERT_EQ(aasm.position, 1000);

  std::string serialized = writer.write(aasm.ToJsonValue());
  ASSERT_EQ(awaited_json, serialized);
}

TEST_F(GeneratedInterfaceTests, MapTest) {
  const char* expected_json =
  "{\"choiceID\":1,\"menuName\":\"Menu name\",\"vrCommands\":{\"one\":\"First value\",\"two\":\"Second value\"}}\n";

  Choice choice;
  ASSERT_FALSE(choice.is_initialized());
  ASSERT_FALSE(choice.is_valid());
  std::map<std::string, std::string> init_map;
  init_map.insert(std::make_pair("one", "First value"));
  init_map.insert(std::make_pair("two", "Second value"));
  choice = Choice(1, "Menu name", init_map);
  ASSERT_TRUE(choice.is_initialized());
  ASSERT_TRUE(choice.is_valid());

  std::string serialized = writer.write(choice.ToJsonValue());
  ASSERT_EQ(expected_json, serialized);
}

TEST_F(GeneratedInterfaceTests, TypedefTest) {
  const char* expected_json =
  "{\"optionalResArrMap\":{\"World\":[\"INVALID_DATA\"]},\"resArrMap\":{\"Hello\":[\"SUCCESS\"]}}\n";

  TdStruct ts;
  ts.resArrMap["Hello"].push_back(R_SUCCESS);
  ts.optionalResArrMap["World"].push_back(R_INVALID_DATA);
  ASSERT_TRUE(ts.is_initialized());
  ASSERT_TRUE(ts.is_valid());
  std::string serialized = writer.write(ts.ToJsonValue());
  ASSERT_EQ(expected_json, serialized);
}

TEST_F(GeneratedInterfaceTests, OverflowedDiagnosticMessageTest) {
  const char* input_json =
      "{\"messageData\":[300, 20],\"messageLength\":2,\"targetID\":5}";
  Value json_value = JsonValue(input_json);
  request::DiagnosticMessage dm(&json_value);
  ASSERT_TRUE(dm.is_initialized());
  ASSERT_FALSE(dm.is_valid());
}

TEST_F(GeneratedInterfaceTests, OverflowedDiagnosticMessageTest64) {
  const char* input_json =
      "{\"messageData\":[10, 123456789123],\"messageLength\":2,\"targetID\":5}";
  Value json_value = JsonValue(input_json);
  request::DiagnosticMessage dm(&json_value);
  ASSERT_TRUE(dm.is_initialized());
  ASSERT_FALSE(dm.is_valid());
}

TEST_F(GeneratedInterfaceTests, TestHandlerCalled) {
  testing::StrictMock<TestRequestHandlerMock> mock;
  request::AddSubMenu add_submenu;
  EXPECT_CALL(mock, HandleAddSubMenu(testing::Ref(add_submenu)))
      .Times(1);
  add_submenu.HandleWith(&mock);
}

TEST_F(GeneratedInterfaceTests, TestFactory) {
  testing::StrictMock<TestRequestHandlerMock> mock;
  Json::Value json_value;
  request::Request* req = request::NewFromJson(&json_value, kAddSubMenuID);
  request::AddSubMenu& add_sub_menu_ref =
      static_cast<request::AddSubMenu&>(*req);
  EXPECT_CALL(mock, HandleAddSubMenu(testing::Ref(add_sub_menu_ref)))
      .Times(1);
  req->HandleWith(&mock);
}

TEST_F(GeneratedInterfaceTests, TestNullableStructMember) {
  TestStructWithNullableParam with_nullable;
  ASSERT_FALSE(with_nullable.is_initialized());
  ASSERT_FALSE(with_nullable.is_valid());
  ASSERT_FALSE(with_nullable.nullableInt.is_valid());
  ASSERT_FALSE(with_nullable.nullableInt.is_null());
  with_nullable.nullableInt.set_to_null();
  ASSERT_TRUE(with_nullable.is_valid());
  ASSERT_TRUE(with_nullable.is_initialized());
  ASSERT_TRUE(with_nullable.nullableInt.is_null());
  ASSERT_TRUE(with_nullable.nullableInt.is_valid());
  ASSERT_TRUE(with_nullable.nullableInt.is_initialized());
}

TEST_F(GeneratedInterfaceTests, TestNullableStructMemberNullInitializationFromJson) {
  const char* input_json =
      "{\"nullableInt\":null}\n";
  Value json_value = JsonValue(input_json);
  TestStructWithNullableParam with_nullable(&json_value);
  ASSERT_TRUE(with_nullable.is_initialized());
  ASSERT_TRUE(with_nullable.is_valid());
  ASSERT_TRUE(with_nullable.nullableInt.is_null());
  std::string result = writer.write(with_nullable.ToJsonValue());
  ASSERT_EQ(input_json, result);
}

TEST_F(GeneratedInterfaceTests, TestNullableStructMemberInitializationFromJson) {
  const char* input_json =
      "{\"nullableInt\":3}\n";
  Value json_value = JsonValue(input_json);
  TestStructWithNullableParam with_nullable(&json_value);
  ASSERT_TRUE(with_nullable.is_initialized());
  ASSERT_TRUE(with_nullable.is_valid());
  ASSERT_FALSE(with_nullable.nullableInt.is_null());
  ASSERT_EQ(3, with_nullable.nullableInt);
  std::string result = writer.write(with_nullable.ToJsonValue());
  ASSERT_EQ(input_json, result);
}

TEST_F(GeneratedInterfaceTests, TestNullableEnumInitialization) {
  TestStructWithNullableStructParam strct_with_nullable;
  strct_with_nullable.nullableEnum = IT_DYNAMIC;
  strct_with_nullable.nonNullableEnum = IT_STATIC;
  ASSERT_TRUE(strct_with_nullable.is_initialized());
  ASSERT_TRUE(strct_with_nullable.is_valid());
  std::string result = writer.write(strct_with_nullable.ToJsonValue());
  const char* awaited_json1 = "{\"nonNullableEnum\":\"STATIC\",\"nullableEnum\":\"DYNAMIC\"}\n";
  ASSERT_EQ(awaited_json1, result);

  strct_with_nullable.nullableEnum.set_to_null();
  ASSERT_TRUE(strct_with_nullable.is_initialized());
  ASSERT_TRUE(strct_with_nullable.is_valid());
  result = writer.write(strct_with_nullable.ToJsonValue());
  const char* awaited_json2 = "{\"nonNullableEnum\":\"STATIC\",\"nullableEnum\":null}\n";
  ASSERT_EQ(awaited_json2, result);
}

TEST_F(GeneratedInterfaceTests, TestStructWithNullableTypedef) {
  StructWithNullableTypedef swntd;
  ASSERT_FALSE(swntd.is_initialized());
  ASSERT_FALSE(swntd.is_valid());
  swntd.nullableTdResult = R_SUCCESS;
  ASSERT_TRUE(swntd.is_initialized());
  ASSERT_TRUE(swntd.is_valid());
  ASSERT_EQ(R_SUCCESS, swntd.nullableTdResult);

  swntd.nullableTdResult.set_to_null();
  const char* awaited_json = "{\"nullableTdResult\":null}\n";
  std::string result = writer.write(swntd.ToJsonValue());
  ASSERT_EQ(awaited_json, result);
}

TEST_F(GeneratedInterfaceTests, TestNullingStructWithNullableMapOfNullableInts) {
  StructWithNullableMapOfNullableInts nmoni;
  ASSERT_FALSE(nmoni.is_initialized());
  ASSERT_FALSE(nmoni.is_valid());
  ASSERT_FALSE(nmoni.nullableMap.is_null());
  nmoni.nullableMap.set_to_null();
  ASSERT_TRUE(nmoni.is_initialized());
  ASSERT_TRUE(nmoni.is_valid());
  ASSERT_TRUE(nmoni.nullableMap.is_null());
  const char* awaited_json = "{\"nullableMap\":null}\n";
  std::string result = writer.write(nmoni.ToJsonValue());
  ASSERT_EQ(awaited_json, result);
}

TEST_F(GeneratedInterfaceTests, TestNullingValueInStructWithNullableMapOfNullableInts) {
  StructWithNullableMapOfNullableInts nmoni;
  ASSERT_FALSE(nmoni.is_initialized());
  ASSERT_FALSE(nmoni.is_valid());
  ASSERT_FALSE(nmoni.nullableMap.is_null());
  nmoni.nullableMap["Hello"].set_to_null();

  ASSERT_TRUE(nmoni.is_initialized());
  ASSERT_TRUE(nmoni.is_valid());
  ASSERT_FALSE(nmoni.nullableMap.is_null());
  ASSERT_TRUE(nmoni.nullableMap["Hello"].is_null());
  const char* awaited_json = "{\"nullableMap\":{\"Hello\":null}}\n";
  std::string result = writer.write(nmoni.ToJsonValue());
  ASSERT_EQ(awaited_json, result);
}

TEST_F(GeneratedInterfaceTests, EmptyStructTests) {
  EmptyStruct e;
  ASSERT_TRUE(e.is_valid());
  ASSERT_FALSE(e.is_initialized());
}

TEST_F(GeneratedInterfaceTests, StructWithOptionalEmptyStructFieldTest) {
  StructWithOptionalEmptyStructField oe;
  ASSERT_TRUE(oe.is_valid());
  ASSERT_FALSE(oe.is_initialized());
}

TEST_F(GeneratedInterfaceTests, StructWithMandatoryEmptyStructFieldTest) {
  StructWithMandatoryEmptyStructField me;
  ASSERT_TRUE(me.is_valid());
  ASSERT_FALSE(me.is_initialized());
}

TEST_F(GeneratedInterfaceTests, EmptyStructJsonTests) {
  EmptyStruct e;
  ASSERT_TRUE(e.is_valid());
  ASSERT_FALSE(e.is_initialized());

  const char* expected_json = "{}\n";
  ASSERT_EQ(expected_json, writer.write(e.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, StructWithOptionalEmptyStructFieldJsonTest) {
  StructWithOptionalEmptyStructField oe;
  ASSERT_TRUE(oe.is_valid());
  ASSERT_FALSE(oe.is_initialized());
  const char* expected_json = "{}\n";
  ASSERT_EQ(expected_json, writer.write(oe.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, StructWithMandatoryEmptyStructFieldJsonTest) {
  StructWithMandatoryEmptyStructField me;
  ASSERT_TRUE(me.is_valid());
  ASSERT_FALSE(me.is_initialized());
  const char* expected_json = "{\"emptyOne\":{}}\n";
  ASSERT_EQ(expected_json, writer.write(me.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, StructWithOneOptionalIntFieldTest) {
  StructWithOneOptionalIntField soo;
  ASSERT_TRUE(soo.is_valid());
  ASSERT_FALSE(soo.is_initialized());
}

TEST_F(GeneratedInterfaceTests, StructWithOneOptionalInitializedIntFieldTest) {
  StructWithOneOptionalIntField soo;
  *soo.optionalInt = 13;
  ASSERT_TRUE(soo.is_valid());
  ASSERT_TRUE(soo.is_initialized());
}

TEST_F(GeneratedInterfaceTests, StructWithOneOptionalIntFieldJsonTest) {
  StructWithOneOptionalIntField soo;
  ASSERT_TRUE(soo.is_valid());
  ASSERT_FALSE(soo.is_initialized());
  const char* expected_json = "{}\n";
  ASSERT_EQ(expected_json, writer.write(soo.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, StructWithOneInitializedOptionalIntFieldJsonTest) {
  const char* input_json = "{\"optionalInt\":11}\n";
  Json::Value json_value = JsonValue(input_json);
  StructWithOneOptionalIntField soo(&json_value);
  ASSERT_TRUE(soo.is_valid());
  ASSERT_TRUE(soo.is_initialized());
  ASSERT_EQ(11, *soo.optionalInt);
  ASSERT_EQ(input_json, writer.write(soo.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, StructWithFieldOfStructThatMightBeEmptyTest) {
  StructWithFieldOfStructThatMightBeEmpty sfme;
  ASSERT_TRUE(sfme.is_valid());
  ASSERT_FALSE(sfme.is_initialized());
  *sfme.fieldThatMightBeEmpty.optionalInt = 5;
  ASSERT_TRUE(sfme.is_valid());
  ASSERT_TRUE(sfme.is_initialized());
}

TEST_F(GeneratedInterfaceTests, StructWithFieldOfStructThatMightBeEmptyJsonNoValueTest) {
  StructWithFieldOfStructThatMightBeEmpty sfme;
  ASSERT_TRUE(sfme.is_valid());
  ASSERT_FALSE(sfme.is_initialized());
  const char* expcected_json = "{\"fieldThatMightBeEmpty\":{}}\n";
  ASSERT_EQ(expcected_json, writer.write(sfme.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, StructWithFieldOfStructThatMightBeEmptyJsonHasValueTest) {
  const char* input_json = "{\"fieldThatMightBeEmpty\":{\"optionalInt\":12}}\n";
  Json::Value json_value = JsonValue(input_json);
  StructWithFieldOfStructThatMightBeEmpty sfme(&json_value);
  ASSERT_TRUE(sfme.is_valid());
  ASSERT_TRUE(sfme.is_initialized());
  ASSERT_EQ(12, *sfme.fieldThatMightBeEmpty.optionalInt);
  ASSERT_EQ(input_json, writer.write(sfme.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, StructWithFieldOfOptionalMapTest) {
  StructWithNullableOptionalMap snom;
  ASSERT_TRUE(snom.is_valid());
  ASSERT_FALSE(snom.is_initialized());
  snom.nullableOptionalIntMap["a"] = 5;
  ASSERT_TRUE(snom.is_valid());
  ASSERT_TRUE(snom.is_initialized());
}

TEST_F(GeneratedInterfaceTests, StructWithFieldOfOptionalMapToJsonTest) {
  StructWithNullableOptionalMap snom;
  ASSERT_TRUE(snom.is_valid());
  ASSERT_FALSE(snom.is_initialized());
  const char* expected_json = "{}\n";
  ASSERT_EQ(expected_json, writer.write(snom.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, StructWithFieldOfOptionalMapNulledToJsonTest) {
  StructWithNullableOptionalMap snom;
  ASSERT_TRUE(snom.is_valid());
  ASSERT_FALSE(snom.is_initialized());
  snom.nullableOptionalIntMap.set_to_null();
  const char* expected_json = "{\"nullableOptionalIntMap\":null}\n";
  ASSERT_EQ(expected_json, writer.write(snom.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, StructWithFieldOfOptionalMapNulledInJsonTest) {
  const char* input_json = "{\"nullableOptionalIntMap\":null}\n";
  Json::Value json_value = JsonValue(input_json);
  StructWithNullableOptionalMap snom(&json_value);
  ASSERT_TRUE(snom.is_valid());
  ASSERT_TRUE(snom.is_initialized());
  ASSERT_TRUE(snom.nullableOptionalIntMap.is_null());
  ASSERT_EQ(input_json, writer.write(snom.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, StructWithFieldOfOptionalMapInitializedInJsonTest) {
  const char* input_json = "{\"nullableOptionalIntMap\":{\"Hello\":2}}\n";
  Json::Value json_value = JsonValue(input_json);
  StructWithNullableOptionalMap snom(&json_value);
  ASSERT_TRUE(snom.is_valid());
  ASSERT_TRUE(snom.is_initialized());
  ASSERT_FALSE(snom.nullableOptionalIntMap.is_null());
  ASSERT_EQ(input_json, writer.write(snom.ToJsonValue()));
}

}  // namespace test
