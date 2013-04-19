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

#include "AppMgr/Application.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

    log4cplus::Logger Application::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("Application"));

    /**
     * \brief Class constructor
     * \param name application name
     * \param appId application id
     * \param protocolVersion protocol version
     */
    Application::Application(const std::string& name , int appId, unsigned int protocolVersion)
        :mNgnMediaScreenAppName("")
        ,m_bIsMediaApplication(false)
        ,mName(name)
        ,mProtocolVersion(protocolVersion)
        ,mAppID(appId)
    {
        addSessionKey(appId);
        LOG4CPLUS_INFO_EXT(mLogger, " Created an application " << name << " application id " << appId);
    }

    /**
     * \brief Copy constructor
     */
    Application::Application( const Application& app )
        :mNgnMediaScreenAppName(app.getNgnMediaScreenAppName())
        ,mName(app.getName())
        ,m_bIsMediaApplication(app.getIsMediaApplication())
        ,mProtocolVersion(app.getProtocolVersion())
        ,mAppID(app.getAppID())
    {
        addSessionKey(app.getAppID());
        mVrSynonyms = app.getVrSynonyms();
    }

    /**
     * \brief Default class destructor
     */
    Application::~Application( )
    {
        LOG4CPLUS_INFO_EXT(mLogger, " Deleted an application " << mName << " application id " << mAppID);
    }

    /**
     * \brief operator ==
     * \param item the item to compare with
     * \return comparison result
     */
    bool Application::operator ==(const Application &item) const
    {
        return this->getAppID() == item.getAppID();
    }

    /**
     * \brief Set application NGN media screen app name
     * \param value application NGN media screen app name
     */
    void Application::setNgnMediaScreenAppName(const std::string& value)
    {
        mNgnMediaScreenAppName = value;
    }

    /**
     * \brief Set application voice recognition synonyms
     * \param value application voice recognition synonyms
     */
    void Application::setVrSynonyms(const std::vector<std::string>& value)
    {
        mVrSynonyms = value;
    }

    /**
     * \brief Set if the application is a media application
     * \param value is the application a media application
     */
    void Application::setIsMediaApplication(bool value)
    {
        m_bIsMediaApplication = value;
    }

    /**
     * \brief retrieve application NGN media screen application name
     * \return application NGN media screen application name
     */
    const std::string& Application::getNgnMediaScreenAppName( ) const
    {
        return mNgnMediaScreenAppName;
    }

    /**
     * \brief retrieve application voice-recognition synonyms
     * \return application voice-recognition synonyms
     */
    const std::vector<std::string>& Application::getVrSynonyms( ) const
    {
        return mVrSynonyms;
    }

    /**
     * \brief retrieve is the application a media application
     * \return is the application a media application
     */
    bool Application::getIsMediaApplication( ) const
    {
        return m_bIsMediaApplication;
    }

    /**
     * \brief retrieve application protocol version
     * \return application protocol version
     */
    const unsigned int &Application::getProtocolVersion() const
    {
        return mProtocolVersion;
    }

    /**
     * \brief retrieve application name
     * \return application name
     */
    const std::string &Application::getName() const
    {
        return mName;
    }

    /**
     * \brief retrieve application ID
     * \return application ID
     */
    const int &Application::getAppID() const
    {
        return mAppID;
    }

    /**
     * \brief add a command to a menu
     * \param commandId command id
     * \param menuId menu id
     */
    void Application::addMenuCommand(const unsigned int &commandId, const unsigned int &menuId)
    {
        mMenuMapping.addCommand(commandId, menuId);
    }

    /**
     * \brief remove a command from a menu(s)
     * \param commandId command id
     */
    void Application::removeMenuCommand(const unsigned int &commandId)
    {
        mMenuMapping.removeCommand(commandId);
    }

    /**
     * \brief find commands within a menu
     * \param menuId menu id
     * \return commands residing within the given menu
     */
    MenuCommands Application::findMenuCommands(const unsigned int &menuId) const
    {
        return mMenuMapping.findCommandsAssignedToMenu(menuId);
    }

    /**
     * \brief add a menu item to the application
     * \param menuId menu id
     * \param menuName menu item name
     * \param position menu item position within the parent menu
     */
    void Application::addMenu(const unsigned int &menuId, const std::string &menuName, const unsigned int *position)
    {
        mMenus.addItem(menuId, menuName, position);
    }

    /**
     * \brief remove a menu item from the application
     * \param menuId menu id
     */
    void Application::removeMenu(const unsigned int &menuId)
    {
        mMenus.removeItem(menuId);
    }

    /**
     * \brief find a menu item from a mapping
     * \param menuId menu id
     * \return a menu item from a mapping
     */
    const MenuValue *Application::findMenu(const unsigned int &menuId)
    {
        return mMenus.findItem(menuId);
    }

    /**
     * \brief gets all application menus
     * \return application menus
     */
    MenuItems Application::getAllMenus() const
    {
        return mMenus.getAllMenuItems();
    }

    /**
     * \brief get count of items
     * \return items count
     */
    size_t Application::getMenusCount() const
    {
        return mMenus.size();
    }

    /**
     * \brief get count of items
     * \return items count
     */
    size_t Application::getMenuCommandsCount() const
    {
        return mMenuMapping.size();
    }

    /**
     * \brief add a command to an application
     * \param commandId command id
     * \param type command type
     * \param params VR or UI params supplied with the AddCommand request
     */
    void Application::addCommand(unsigned int commandId, CommandType type, CommandParams params)
    {
        mCommandMapping.addCommand(commandId, type, params);
    }

    /**
     * \brief remove a command from application
     * \param commandId command id
     * \param type a type of a command
     */
    void Application::removeCommand(unsigned int commandId, CommandType type)
    {
        mCommandMapping.removeCommand(commandId, type);
    }

    /**
     * \brief finds commands in application
     * \param commandId command id
     * \return command list
     */
    Commands Application::findCommands(unsigned int commandId) const
    {
        return mCommandMapping.findCommands(commandId);
    }

    /**
     * \brief gets all application commands
     * \return application commands
     */
    Commands Application::getAllCommands() const
    {
        return mCommandMapping.getAllCommands();
    }

    /**
     * \brief get count of items
     * \return items count
     */
    size_t Application::getCommandsCount() const
    {
        return mCommandMapping.size();
    }

    /**
     * \brief retrieve types associated with command id in current application
     * \param commandId command id to search for types
     * \return input container of command types to be filled with result
     */
    CommandTypes Application::getCommandTypes(unsigned int commandId) const
    {
        return mCommandMapping.getTypes(commandId);
    }

    /**
     * \brief get count of unresponsed requests associated with the given command id
     * \param cmdId id of command we need to count unresponded requests for
     * \return unresponded requests count
     */
    unsigned int Application::getUnrespondedRequestCount(const unsigned int &cmdId) const
    {
        return mCommandMapping.getUnrespondedRequestCount(cmdId);
    }

    /**
     * \brief increment count of unresponsed requests associated with the given command id
     * \param cmdId id of command we need to increment unresponded request count for
     * \return unresponded requests count after the operation
     */
    unsigned int Application::incrementUnrespondedRequestCount(const unsigned int &cmdId)
    {
        return mCommandMapping.incrementUnrespondedRequestCount(cmdId);
    }

    /**
     * \brief decrement count of unresponsed requests associated with the given command id
     * \param cmdId id of command we need to decrement unresponded request count for
     * \return unresponded requests count after the operation
     */
    unsigned int Application::decrementUnrespondedRequestCount(const unsigned int &cmdId)
    {
        return mCommandMapping.decrementUnrespondedRequestCount(cmdId);
    }

    /**
     * \brief add session key to a list of session keys
     * \param sessionKey session key
     */
    void Application::addSessionKey(const int &sessionKey)
    {
        mSessionKeys.push_back(sessionKey);
    }

    /**
     * \brief remove session key from a list of session keys
     * \param sessionKey session key
     */
    void Application::removeSessionKey(const int &sessionKey)
    {
        mSessionKeys.remove(sessionKey);
    }

     /**
     * \brief retrieve application audio streaming state
     * \return application audio streaming state
     */
    const NsSmartDeviceLinkRPCV2::AudioStreamingState& Application::getApplicationAudioStreamingState( ) const
    {
        return mAudioStreamingState;
    }

    /**
     * \brief retrieve application system context
     * \return application system context
     */
    const NsSmartDeviceLinkRPCV2::SystemContext &Application::getSystemContext() const
    {
        return mSystemContext;
    }

    /**
     * \brief retrieve aplication HMI status level
     * \return HMI status level of application
     */
    const NsSmartDeviceLinkRPCV2::HMILevel::HMILevelInternal& Application::getApplicationHMIStatusLevel( ) const
    {
        return mHMIStatusLevel;
    }

    /**
     * \brief Set application audio streaming state
     * \param streamingState audio streaming state of application
     */
    void Application::setApplicationAudioStreamingState( const NsSmartDeviceLinkRPCV2::AudioStreamingState& streamingState )
    {
        mAudioStreamingState = streamingState;
    }

    /**
     * \brief set application system context
     * \param application system context
     */
    void Application::setSystemContext(NsSmartDeviceLinkRPCV2::SystemContext value)
    {
        mSystemContext = value;
    }

    /**
     * \brief Set application HMI status level
     * \param hmiLevel HMI status level of application
     */
    void Application::setApplicationHMIStatusLevel( const NsSmartDeviceLinkRPCV2::HMILevel::HMILevelInternal& hmiLevel )
    {
        mHMIStatusLevel = hmiLevel;
    }

    void Application::addPersistentFile(const std::string& file)
    {
        persistentFiles.push_back(file);
    }

    void Application::removePersistentFile(const std::string& file)
    {
        std::vector<std::string>::iterator i = std::find(persistentFiles.begin()
            , persistentFiles.end(), file);
        if (i != persistentFiles.end())
        {
            persistentFiles.erase(i);
        }
    }

    bool Application::isPersistentFile(const std::string& file)
    {
        std::vector<std::string>::iterator i = std::find(persistentFiles.begin()
            , persistentFiles.end(), file);
        if (i != persistentFiles.end())
        {
            return true;
        }
        return false;
    }

    int Application::getDeviceHandle() const
    {
        return mDeviceHandle;
    }

    void Application::setDeviceHandle(int deviceHandle)
    {
        mDeviceHandle = deviceHandle;
    }

}
