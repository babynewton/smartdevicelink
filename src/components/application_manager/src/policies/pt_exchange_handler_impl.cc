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

#include "application_manager/policies/pt_exchange_handler_impl.h"
#include <unistd.h>

namespace policy {

log4cxx::LoggerPtr PTExchangeHandlerImpl::logger_ = log4cxx::LoggerPtr(
    log4cxx::Logger::getLogger("PTExchangeHandlerImpl"));

PTExchangeHandlerImpl::PTExchangeHandlerImpl(PolicyHandler* handler)
    : policy_handler_(handler) {
  DCHECK(policy_handler_);
}

PTExchangeHandlerImpl::~PTExchangeHandlerImpl() {
  policy_handler_ = NULL;
}

bool PTExchangeHandlerImpl::StartExchange() {
  LOG4CXX_INFO(logger_, "PolicyHandler::StartNextRetry");
  DCHECK(policy_handler_);
  if (!policy_handler_->policy_manager()) {
    LOG4CXX_WARN(logger_, "The shared library of policy is not loaded");
    return false;
  }

  BinaryMessageSptr pt_snapshot = policy_handler_->policy_manager()
      ->RequestPTUpdate();
  if (pt_snapshot) {
    policy_handler_->SendMessageToSDK(*pt_snapshot);

    int timeout = policy_handler_->policy_manager()->TimeoutExchange();
    int seconds = policy_handler_->policy_manager()->NextRetryTimeout();
    LOG4CXX_DEBUG(logger_,
                  "Timeout response: " << timeout << " Next try: " << seconds);
    if (timeout > 0) {
      sleep(timeout);
      policy_handler_->policy_manager()->OnExceededTimeout();
    }
    if (seconds > 0) {
      sleep(seconds);
      StartExchange();
    } else {
      LOG4CXX_INFO(logger_, "End retry sequence. Update PT was not received");
    }
  }
  return true;
}

}  //  namespace policy
