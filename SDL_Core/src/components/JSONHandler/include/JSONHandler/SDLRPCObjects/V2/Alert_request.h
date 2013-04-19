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

#ifndef NSSMARTDEVICELINKKRPCV2_ALERT_REQUEST_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_ALERT_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "SoftButton.h"
#include "TTSChunk.h"
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

///  Shows an alert which typically consists of text-to-speech message and text on the display. At least either alertText1, alertText2 or TTSChunks need to be provided.

  class Alert_request : public NsSmartDeviceLinkRPC::SDLRPCMessage
  {
  public:
  
    Alert_request(const Alert_request& c);
    Alert_request(void);
    
    virtual ~Alert_request(void);
  
    Alert_request& operator =(const Alert_request&);
  
    bool checkIntegrity(void);

    const std::string* get_alertText1(void) const;
    const std::string* get_alertText2(void) const;
    const std::string* get_alertText3(void) const;
    const std::vector<TTSChunk>* get_ttsChunks(void) const;
    const unsigned int* get_duration(void) const;
    const bool* get_playTone(void) const;
    const std::vector<SoftButton>* get_softButtons(void) const;

    void reset_alertText1(void);
    bool set_alertText1(const std::string& alertText1_);
    void reset_alertText2(void);
    bool set_alertText2(const std::string& alertText2_);
    void reset_alertText3(void);
    bool set_alertText3(const std::string& alertText3_);
    void reset_ttsChunks(void);
    bool set_ttsChunks(const std::vector<TTSChunk>& ttsChunks_);
    void reset_duration(void);
    bool set_duration(unsigned int duration_);
    void reset_playTone(void);
    bool set_playTone(bool playTone_);
    void reset_softButtons(void);
    bool set_softButtons(const std::vector<SoftButton>& softButtons_);

  private:
  
    friend class Alert_requestMarshaller;


///  The first line of the alert text field
      std::string* alertText1;	//!< (500)

///  The second line of the alert text field
      std::string* alertText2;	//!< (500)

///  The optional third line of the alert text field
      std::string* alertText3;	//!< (500)

/**
     An array of text chunks of type TTSChunk. See TTSChunk
     The array must have at least one item
*/
      std::vector<TTSChunk>* ttsChunks;	//!<   [%s..%s] 

/**
     Timeout in milliseconds.
     Typical timeouts are 3-5 seconds
     If omitted, timeout is set to 5s.
*/
      unsigned int* duration;	//!<  (3000,10000)

/**
     Defines if tone should be played. Tone is played before TTS.
     If omitted, no tone is played.
*/
      bool* playTone;

/**
     App defined SoftButtons.
     If omitted on supported displays, the displayed alert shall not have any SoftButtons.
*/
      std::vector<SoftButton>* softButtons;	//!<   [%s..%s] 
  };

}

#endif
