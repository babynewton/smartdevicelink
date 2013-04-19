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

#include "../include/JSONHandler/SDLRPCObjects/V2/ListFiles_response.h"
#include "ListFiles_responseMarshaller.h"

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
ListFiles_response& ListFiles_response::operator =(const ListFiles_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    filenames = c.filenames ? new std::vector<std::string>(c.filenames[0]) : 0;
    spaceAvailable = c.spaceAvailable;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}

ListFiles_response::~ListFiles_response(void)
{
    if (filenames)
    {
        delete filenames;
    }
}

ListFiles_response::ListFiles_response(const ListFiles_response& c)
{
    *this = c;
}

bool ListFiles_response::checkIntegrity(void)
{
    return ListFiles_responseMarshaller::checkIntegrity(*this);
}

ListFiles_response::ListFiles_response(void)
    : NsSmartDeviceLinkRPC::SDLRPCResponse(PROTOCOL_VERSION)
    , filenames(0)
{}

bool ListFiles_response::set_filenames(const std::vector<std::string>& filenames_)
{
    unsigned int i = filenames_.size();
    if (i > 1000 || i < 0) // TODO(AK): unsigned int i < 0
    {
        return false;
    }
    while (i--)
    {
        if (filenames_[i].length() > 500)
        {
            return false;
        }
    }
    delete filenames;
    filenames = 0;

    filenames = new std::vector<std::string>(filenames_);
    return true;
}

void ListFiles_response::reset_filenames(void)
{
    if (filenames)
    {
        delete filenames;
    }
    filenames = 0;
}

bool ListFiles_response::set_spaceAvailable(unsigned int spaceAvailable_)
{
    if (spaceAvailable_ > 2000000000)
    {
        return false;
    }
    spaceAvailable = spaceAvailable_;
    return true;
}

const std::vector<std::string>* ListFiles_response::get_filenames(void) const
{
    return filenames;
}

unsigned int ListFiles_response::get_spaceAvailable(void) const
{
    return spaceAvailable;
}
