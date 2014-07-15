/**
 * Copyright (c) 2013, Ford Motor Company
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

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "config_profile/profile.h"
#include "utils/logger.h"

#include "utils/file_system_tests.h"
#include "utils/data_time_tests.h"
#include "utils/prioritized_queue_tests.h"
#include "protocol_handler/protocol_handler_tm_test.h"
#include "application_manager/formatters_commands.h"
#include "SmartObjectDraftTest.h"
#include "SmartObjectInvalidTest.h"
#include "SmartObjectStressTest.h"
#include "SmartObjectUnitTest.h"
#include "TSharedPtrTest.h"
//#include "media_manager/media_manager_impl_test.h"
//#include "jsoncpp/json_reader_test.h"

// #include "json_handler/smart_schema_draft_test.h"
// #include "SmartObjectConvertionTimeTest.h"
// #include "request_watchdog/request_watchdog_test.h"
// #include "json_handler/formatters/formatter_test_helper.h"
// #include "json_handler/formatters/formatter_json_alrpcv1_test.h"
// #include "json_handler/formatters/formatter_json_alrpcv2_test.h"
// #include "json_handler/formatters/formatter_json_rpcv2_test.h"

#ifdef TESTS_WITH_HMI
#include "hmi_message_handler/hmi_message_handler.h"
#include "life_cycle.cc"
#include "rpc/admin_app_test.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_options.h"
#include "rpc/test_app_manager.h"
#endif

// #define QT_HMI

#ifdef __cplusplus
extern "C" void __gcov_flush();
#endif

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);

  profile::Profile::instance()->config_file_name("smartDeviceLink.ini");
  INIT_LOGGER("log4cxx.properties");

#ifdef TESTS_WITH_HMI
  test::AdminAppTest app;

  app.Run();
  sleep(5);
#endif
  int result = RUN_ALL_TESTS();

#if defined(__cplusplus) and defined(GCOV_ENABLED)
  __gcov_flush();
#endif

  sleep(2);
  return result;
}
