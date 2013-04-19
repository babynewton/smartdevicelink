//
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
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#include "../include/JSONHandler/SDLRPCObjects/V2/ReadDID_response.h"
#include "ReadDID_responseMarshaller.h"
#include "VehicleDataResultCodeMarshaller.h"

namespace
{
    const int PROTOCOL_VERSION = 2;
}

/*
  interface Ford Sync RAPI
  version   2.0O
  date      2012-11-02
  generated at  Thu Jan 24 06:36:23 2013
  source stamp  Thu Jan 24 06:35:41 2013
  author    RC
*/

using namespace NsSmartDeviceLinkRPCV2;
ReadDID_response& ReadDID_response::operator =(const ReadDID_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;
    dataResult = c.dataResult ? new std::vector<VehicleDataResultCode>(c.dataResult[0]) : 0;
    data = c.data ? new std::vector<std::string>(c.data[0]) : 0;

    return *this;
}

ReadDID_response::~ReadDID_response(void)
{
    if (dataResult)
    {
        delete dataResult;
    }
    if (data)
    {
        delete data;
    }
}

ReadDID_response::ReadDID_response(const ReadDID_response& c)
{
    *this = c;
}

bool ReadDID_response::checkIntegrity(void)
{
    return ReadDID_responseMarshaller::checkIntegrity(*this);
}

ReadDID_response::ReadDID_response(void)
    : NsSmartDeviceLinkRPC::SDLRPCResponse(PROTOCOL_VERSION)
    , dataResult(0)
    , data(0)
{
}

bool ReadDID_response::set_dataResult(const std::vector<VehicleDataResultCode>& dataResult_)
{
    unsigned int i = dataResult_.size();
    if (i > 1000 || i < 0)  // TODO(AK): unsigned int i < 0
    {
        return false;
    }
    while (i--)
    {
        if (!VehicleDataResultCodeMarshaller::checkIntegrityConst(dataResult_[i]))
        {
            return false;
        }
    }
    delete dataResult;
    dataResult = 0;

    dataResult = new std::vector<VehicleDataResultCode>(dataResult_);
    return true;
}

void ReadDID_response::reset_dataResult(void)
{
    if (dataResult)
    {
        delete dataResult;
    }
    dataResult = 0;
}

bool ReadDID_response::set_data(const std::vector<std::string>& data_)
{
    unsigned int i = data_.size();
    if (i > 1000 || i < 0)  // TODO(AK): unsigned int i < 0
    {
        return false;
    }
    while (i--)
    {
        if (data_[i].length() > 5000)
        {
            return false;
        }
    }
    delete data;
    data = 0;

    data = new std::vector<std::string>(data_);
    return true;
}

void ReadDID_response::reset_data(void)
{
    if (data)
    {
        delete data;
    }
    data = 0;
}

const std::vector<VehicleDataResultCode>* ReadDID_response::get_dataResult(void) const
{
    return dataResult;
}

const std::vector<std::string>* ReadDID_response::get_data(void) const
{
    return data;
}

