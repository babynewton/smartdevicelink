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

#ifndef SRC_COMPONENTS_CONFIG_PROFILE_INCLUDE_CONFIG_PROFILE_PROFILE_H_
#define SRC_COMPONENTS_CONFIG_PROFILE_INCLUDE_CONFIG_PROFILE_PROFILE_H_

#include <string>
#include <vector>
#include <stdint.h>
#include "utils/macro.h"
#include "utils/singleton.h"

namespace profile {

/**
 * The Profile class
 */
class Profile : public utils::Singleton<Profile> {
  public:
    // Methods section

    /**
     * Destructor
     *
     * Unimplemented to avoid misusing
     *
     */
    virtual ~Profile();

    /**
     * @brief Returns the path to the config file
     */
    const std::string& config_file_name() const;

    /**
     * @brief Sets the path to the config file
     */
    void config_file_name(const std::string& fileName);

    /**
      * @brief Returns true if HMI should be started, otherwise false
      */
    bool launch_hmi() const;

    /**
     * @brief Returns server address
     */
    const std::string& server_address() const;

    /**
     * @brief Returns server port
     */
    const uint16_t& server_port() const;

    /**
     * @brief Returns port for video streaming
     */
    const uint16_t& video_streaming_port() const;

    /**
      * @brief Returns port for audio streaming
      */
    const uint16_t& audio_streaming_port() const;

    /**
     * @brief Returns policies file name
     */
    const std::string& policies_file_name() const;

    /**
     * @brief Returns hmi capabilities file name
     */
    const std::string& hmi_capabilities_file_name() const;

    /**
     * @brief Returns help promt vector
     */
    const std::vector<std::string>& help_prompt() const;

    /**
     * @brief Returns help promt vector
     */
    const std::vector<std::string>& time_out_promt() const;

    /**
     * @brief Returns vr commands default for all apps
     * such as Help.
     */
    const std::vector<std::string>& vr_commands() const;

    /**
     * @brief Maximum command id available for mobile app
     */
    const uint32_t& max_cmd_id() const;

    /**
     * @brief Default timeout for waiting for response to mobile app
     */
    const uint32_t& default_timeout() const;

    /**
     * @brief Default timeout for waiting for resuming
     */
    const uint32_t& app_resuming_timeout() const;

    /**
     * @brief Returns desirable thread stack size
     */
    const uint64_t& thread_min_stack_size() const;

    /**
      * @brief Returns true if audio mixing is supported
      */
    bool is_mixing_audio_supported() const;

    /**
      * @brief Returns true if video re-decoding enabled
      */
    bool is_redecoding_enabled() const;

    /**
      * @brief Returns title for Vr Help
      */
    const std::string& vr_help_title() const;

    /**
      * @brief Returns application directory quota size
      */
    const uint32_t& app_dir_quota() const;

    /**
      * @brief Returns the video server type
      */
    const std::string& video_server_type() const;

    /**
      * @brief Returns the audio server type
      */
    const std::string& audio_server_type() const;

    /**
      * @brief Returns the video pipe path
      */
    const std::string& named_video_pipe_path() const;

     /**
      * @brief Returns the audio pipe path
      */
     const std::string& named_audio_pipe_path() const;

    /**
     * @brief Returns time scale for max amount of requests for application
     * in hmi level none.
     */
    const uint32_t& app_hmi_level_none_time_scale() const;

    /**
      * @brief Returns path to testing file to which redirects video stream
      */
    const std::string& video_stream_file() const;

    /**
      * @brief Returns path to testing file to which redirects audio stream
      */
    const std::string& audio_stream_file() const;

    /**
     * @brief Returns allowable max amount of requests per time scale for
     * application in hmi level none
     *
     */
    const uint32_t& app_hmi_level_none_time_scale_max_requests() const;

    /**
     * @brief Returns application time scale for max amount of requests per it.
     */
    const uint32_t& app_time_scale() const;

    /**
     * @brief Returns allowable max amount of requests per application
     * time scale
     */
    const uint32_t& app_time_scale_max_requests() const;

    /**
     * @brief Returns allowable amount of the system pending requests
     */
    const uint32_t& pending_requests_amount() const;

    /**
     * @brief Returns Max allowed number of PutFile requests for one
     * application in NONE
     */
    const uint32_t& put_file_in_none() const;

    /**
     * @brief Returns Max allowed number of DeleteFile requests for one
     * application in NONE
     */
    const uint32_t& delete_file_in_none() const;

    /**
     * @brief Returns Max allowed number of ListFiles requests for one
     * application in NONE
     */
    const uint32_t& list_files_in_none() const;

    /*
     * @brief Returns file name for storing applications data
     */
    const std::string& app_info_storage() const;

    /*
     * @brief Heartbeat timeout before closing connection
     */
    const int32_t heart_beat_timeout() const;

    /*
     * @brief Timeout in transport manager before disconnect
     */
    uint32_t transport_manager_disconnect_timeout() const;

    /*
     * @brief Returns true if last state singleton is used
     */
    bool use_last_state() const;

    /**
     * @brief Returns supported diagnostic modes
     */
    const std::vector<uint32_t>& supported_diag_modes() const;

    // Members section

  protected:
    // Methods section

    // Members section

  private:
    // Methods section

    /**
     * Default constructor
     *
     * Unimplemented to avoid misusing
     *
     */
    Profile();

    /*
     * @brief Updates all related values from ini file
     */
    void UpdateValues();

    /**
     * @brief Reads a boolean value from the profile
     *
     * @param value      The value to return
     * @param pSection   The section to read the value in
     * @param pKey       The key whose value needs to be read out
     *
     * @return FALSE if could not read the value out of the profile
     * (then the value is not changed)
     */
    bool ReadValue(bool* value,
                   const char* const pSection,
                   const char* const pKey) const;

    /**
     * @brief Reads a string value from the profile
     *
     * @param value      The value to return
     * @param pSection   The section to read the value in
     * @param pKey       The key whose value needs to be read out
     *
     * @return FALSE if could not read the value out of the profile
     * (then the value is not changed)
     */
    bool ReadValue(std::string* value,
                   const char* const pSection,
                   const char* const pKey) const;


    /**
     * @brief Reads a string value from the profile
     *
     * @param value         Result value
     * @param default_value Value to use key wasn't found
     * @param pSection      The section to read the value in
     * @param pKey          The key whose value needs to be read out
     *
     * @return FALSE if could not read the value out of the profile
     * (then the value is not changed)
     */
    bool ReadStringValue(std::string* value,
                         const char* default_value,
                         const char* const pSection,
                         const char* const pKey) const;

    /**
     * @brief Reads an int32_t value from the profile
     *
     * @param value         Result value
     * @param default_value Value to use key wasn't found
     * @param pSection      The section to read the value in
     * @param pKey          The key whose value needs to be read out
     *
     * @return FALSE if could not read the value out of the profile
     * (then the value is not changed)
     */
    bool ReadIntValue(int32_t* value,
                      int32_t  default_value,
                      const char* const pSection,
                      const char* const pKey) const;

    // Members section
    bool                            launch_hmi_;
    std::string                     config_file_name_;
    std::string                     server_address_;
    uint16_t                        server_port_;
    uint16_t                        video_streaming_port_;
    uint16_t                        audio_streaming_port_;
    std::string                     policies_file_name_;
    std::string                     hmi_capabilities_file_name_;
    std::vector<std::string>        help_prompt_;
    std::vector<std::string>        time_out_promt_;
    std::vector<std::string>        vr_commands_;
    uint64_t                        min_tread_stack_size_;
    bool                            is_mixing_audio_supported_;
    bool                            is_redecoding_enabled_;
    uint32_t                        max_cmd_id_;
    uint32_t                        default_timeout_;
    uint32_t                        app_resuming_timeout_;
    std::string                     vr_help_title_;
    uint32_t                        app_dir_quota_;
    std::string                     video_consumer_type_;
    std::string                     audio_consumer_type_;
    std::string                     named_video_pipe_path_;
    std::string                     named_audio_pipe_path_;
    uint32_t                        app_hmi_level_none_time_scale_max_requests_;
    uint32_t                        app_hmi_level_none_requests_time_scale_;
    std::string                     video_stream_file_;
    std::string                     audio_stream_file_;
    uint32_t                        app_time_scale_max_requests_;
    uint32_t                        app_requests_time_scale_;
    uint32_t                        pending_requests_amount_;
    uint32_t                        put_file_in_none_;
    uint32_t                        delete_file_in_none_;
    uint32_t                        list_files_in_none_;
    std::string                     app_info_storage_;
    int32_t                         heart_beat_timeout_;
    uint32_t                        transport_manager_disconnect_timeout_;
    bool                            use_last_state_;
    std::vector<uint32_t>           supported_diag_modes_;

    DISALLOW_COPY_AND_ASSIGN(Profile);

    FRIEND_BASE_SINGLETON_CLASS(Profile);
};

}  //  namespace profile

#endif  // SRC_COMPONENTS_CONFIG_PROFILE_INCLUDE_CONFIG_PROFILE_PROFILE_H_
