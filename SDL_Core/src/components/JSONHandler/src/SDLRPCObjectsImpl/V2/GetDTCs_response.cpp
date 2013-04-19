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

#include "../include/JSONHandler/SDLRPCObjects/V2/GetDTCs_response.h"
#include "GetDTCs_responseMarshaller.h"
#include "DTCMarshaller.h"

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
GetDTCs_response& GetDTCs_response::operator =(const GetDTCs_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;
    dtcList = c.dtcList ? new std::vector<DTC>(c.dtcList[0]) : 0;

    return *this;
}

GetDTCs_response::~GetDTCs_response(void)
{
    if (dtcList)
    {
        delete dtcList;
    }
}

GetDTCs_response::GetDTCs_response(const GetDTCs_response& c)
{
    *this = c;
}

bool GetDTCs_response::checkIntegrity(void)
{
    return GetDTCs_responseMarshaller::checkIntegrity(*this);
}

GetDTCs_response::GetDTCs_response(void)
    : NsSmartDeviceLinkRPC::SDLRPCResponse(PROTOCOL_VERSION)
    , dtcList(0)
{}

bool GetDTCs_response::set_dtcList(const std::vector<DTC>& dtcList_)
{
    unsigned int i = dtcList_.size();
    if (i > 100 || i < 1)
    {
        return false;
    }

    while (i--)
    {
        if (!DTCMarshaller::checkIntegrityConst(dtcList_[i]))
        {
            return false;
        }
    }

    delete dtcList;
    dtcList = 0;

    dtcList = new std::vector<DTC>(dtcList_);
    return true;
}

void GetDTCs_response::reset_dtcList(void)
{
    if (dtcList)
    {
        delete dtcList;
    }
    dtcList = 0;
}

const std::vector<DTC>* GetDTCs_response::get_dtcList(void) const
{
    return dtcList;
}

