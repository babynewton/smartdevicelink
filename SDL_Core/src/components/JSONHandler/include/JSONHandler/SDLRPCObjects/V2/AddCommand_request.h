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

#ifndef NSSMARTDEVICELINKKRPCV2_ADDCOMMAND_REQUEST_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_ADDCOMMAND_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "Image.h"
#include "MenuParams.h"
#include "JSONHandler/SDLRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPCV2
{

/**
     Adds a command to the in application menu.
     Either menuParams or vrCommands must be provided.
*/

  class AddCommand_request : public NsSmartDeviceLinkRPC::SDLRPCMessage
  {
  public:
  
    AddCommand_request(const AddCommand_request& c);
    AddCommand_request(void);
    
    virtual ~AddCommand_request(void);
  
    AddCommand_request& operator =(const AddCommand_request&);
  
    bool checkIntegrity(void);

    unsigned int get_cmdID(void) const;
    const MenuParams* get_menuParams(void) const;
    const std::vector<std::string>* get_vrCommands(void) const;
    const Image* get_cmdIcon(void) const;

    bool set_cmdID(unsigned int cmdID_);
    void reset_menuParams(void);
    bool set_menuParams(const MenuParams& menuParams_);
    void reset_vrCommands(void);
    bool set_vrCommands(const std::vector<std::string>& vrCommands_);
    void reset_cmdIcon(void);
    bool set_cmdIcon(const Image& cmdIcon_);

  private:
  
    friend class AddCommand_requestMarshaller;


///  unique ID of the command to add.
      unsigned int cmdID;	//!<  (0,2000000000)

///  Optional sub value containing menu parameters
      MenuParams* menuParams;

/**
     An array of strings to be used as VR synonyms for this command.
     If this array is provided, it may not be empty.
*/
      std::vector<std::string>* vrCommands;	//!<   [%s..%s] (99)

/**
     Image struct determining whether static or dynamic icon.
     If omitted on supported displays, no (or the default if applicable) icon shall be displayed.
*/
      Image* cmdIcon;
  };

}

#endif
