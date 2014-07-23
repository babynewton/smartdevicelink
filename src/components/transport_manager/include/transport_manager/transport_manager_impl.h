/**
 * \file transport_manager_impl.h
 * \brief TransportManagerImpl class header file.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_IMPL_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_IMPL_H_

#include <pthread.h>

#ifdef RWLOCK_SUPPORT
#  if (defined(OS_LINUX) && (defined(__USE_UNIX98) || defined(__USE_XOPEN2K))) || \
      (defined(OS_QNX)   && (defined(__EXT_POSIX1_200112)))
#  define USE_RWLOCK
#  endif
#endif

#include <queue>
#include <map>
#include <list>
#include <algorithm>

#include "utils/timer_thread.h"
#include "transport_manager/common.h"
#include "transport_manager/transport_manager.h"
#include "transport_manager/transport_manager_listener.h"
#include "transport_manager/transport_adapter/transport_adapter_listener_impl.h"
#ifdef TIME_TESTER
#include "transport_manager/time_metric_observer.h"
#endif  // TIME_TESTER

using ::transport_manager::transport_adapter::TransportAdapterListener;

namespace transport_manager {

/**
 * @enum Transport manager states.
 */
enum {
  E_SUCCESS = 0,
  E_TM_IS_NOT_INITIALIZED,
  E_INVALID_HANDLE,
  E_CONNECTION_IS_TO_SHUTDOWN,
  E_CONNECTION_EXISTS,
  E_ADAPTER_EXISTS,
  E_ADAPTERS_FAIL,
  E_INTERNAL_ERROR
};

/**
 * @brief Implementation of transport manager.
 */
class TransportManagerImpl : public TransportManager {
 public:
  /**
   * @brief Hold connection parameters.
   */
  struct Connection {
    ConnectionUID id;
    DeviceUID device;
    ApplicationHandle application;
  };

 private:
  /**
   * @brief Structure that contains internal connection parameters
   */
  struct ConnectionInternal: public Connection {
    TransportManagerImpl* transport_manager;
    TransportAdapter* transport_adapter;
    typedef timer::TimerThread<ConnectionInternal> TimerInternal;
    typedef utils::SharedPtr<TimerInternal> TimerInternalSharedPointer;
    TimerInternalSharedPointer timer;
    bool shutDown;
    DeviceHandle device_handle_;
    int messages_count;

    ConnectionInternal(TransportManagerImpl* transport_manager,
                       TransportAdapter* transport_adapter,
                       const ConnectionUID& id, const DeviceUID& dev_id,
                       const ApplicationHandle& app_id,
                       const DeviceHandle& device_handle);
    void DisconnectFailedRoutine();
  };
 public:

  /**
   * @brief Destructor.
   **/
  virtual ~TransportManagerImpl(void);

  /**
   * @brief Initialize transport manager.
   *
   * @return Code error.
   */
  virtual int Init();

  /**
   * @brief Start scanning for new devices.
   *
   * @return Code error.
   **/
  virtual int SearchDevices(void);

  /**
   * @brief Connect to all applications discovered on device.
   *
   * @param device_id Handle of device to connect to.
   *
   * @return Code error.
   **/
  virtual int ConnectDevice(const DeviceHandle& device_id);

  /**
   * @brief Disconnect from all applications connected on device.
   *
   * @param device_id Handle of device to Disconnect from.
   *
   * @return Code error.
   **/
  virtual int DisconnectDevice(const DeviceHandle& device_id);

  /**
   * @brief Disconnect from applications connected on device by connection
   *unique identifier.
   *
   * @param connection Connection unique identifier.
   *
   * @return Code error.
   **/
  virtual int Disconnect(const ConnectionUID& connection_id);

  /**
   * @brief Disconnect and clear all unreceived data.
   *
   * @param connection Connection unique identifier.
   */
  virtual int DisconnectForce(const ConnectionUID& connection_id);
  /**
   * @brief Post new message in queue for massages destined to device.
   *
   * @param message Smart pointer to the raw massage.
   *
   * @return Code error.
   **/
  virtual int SendMessageToDevice(const RawMessageSptr message);

  /**
   * @brief Post event in the event queue.
   *
   * @param event Current event information.
   *
   * @return Code error.
   **/
  virtual int ReceiveEventFromDevice(const TransportAdapterEvent& event);

  /**
   * @brief Post listener to the container of transport manager listeners.
   *
   * @param listener Pointer to the transport manager listener.
   *
   * @return Code error.
   **/
  virtual int AddEventListener(TransportManagerListener* listener);

  virtual int Stop();

  /**
   * @brief Add device adapter to the container of device adapters.
   *
   * @param transport_adapter Smart pointer to the device adapter.
   *
   * @return Code error.
   **/
  virtual int AddTransportAdapter(
      transport_adapter::TransportAdapter* transport_adapter);

  /**
   * @brief Remove device from the container that hold devices.
   *
   * @param device Handle of device.
   *
   * @return Code error.
   **/
  virtual int RemoveDevice(const DeviceHandle& device);

  /**
   * @brief Turns on or off visibility of SDL to mobile devices
   * when visibility is ON (on_off = true) mobile devices are able to connect
   * otherwise ((on_off = false)) SDL is not visible from outside
   *
   * @return Code error.
   */
  virtual int Visibility(const bool& on_off) const;

  /**
   * @brief Updates total device list with info from specific transport adapter.
   * @param ta Transport adapter
   */
  void UpdateDeviceList(TransportAdapter* ta);

#ifdef TIME_TESTER
  /**
   * @brief Setup observer for time metric.
   *
   * @param observer - pointer to observer
   */
  void SetTimeMetricObserver(TMMetricObserver* observer);
#endif  // TIME_TESTER


  /**
   * @brief Constructor.
   **/
  TransportManagerImpl();

 protected:

  template <class Proc, class... Args>
  void RaiseEvent(Proc proc, Args... args) {
    for (TransportManagerListenerList::iterator it =
             transport_manager_listener_.begin();
         it != transport_manager_listener_.end(); ++it) {
      ((*it)->*proc)(args...);
    }
  }

  /**
   * @brief Put massage in the container of massages.
   *
   * @param message Smart pointer to the raw massage.
   **/
  void PostMessage(const RawMessageSptr message);

  /**
   * @brief update message in queue
   *
   * @param message shared pointer to raw massage
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  /*not clear when this function shall be used
   * void updateMessage(const RawMessageSptr old_message, const RawMessageSptr
   * new_message);*/

  /**
   * @brief Remove message from the container of massages.
   *
   * @param message Smart pointer to the raw massage.
   **/
  void RemoveMessage(const RawMessageSptr message);

  /**
   * @brief Post event to the container of events.
   *
   * @param event Event of device adapter.
   **/
  void PostEvent(const TransportAdapterEvent& event);

  /**
   * @brief Type definition of container that holds smart pointer to the raw
   *massages.
   **/
  typedef std::list<RawMessageSptr> MessageQueue;

  /**
   * @brief Type definition of container that holds events of device adapters.
   **/
  typedef std::vector<TransportAdapterEvent> EventQueue;

  static void* MessageQueueStartThread(void* data);

  /**
   * @brief Scan message's queue and pull messages according to priority and
   *serial number
   *
   * @param
   *
   * @see @ref components_transportmanager_client_connection_management
   */
  void MessageQueueThread(void);

  /**
   * @brief Launch EventListenerThread(void).
   */
  static void* EventListenerStartThread(void*);
  /**
   * @brief wait until event happens
   *
   * @param
   *
   * @see @ref components_transportmanager_client_connection_management
   */
  void EventListenerThread(void);

  /**
   * @brief store messages
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  MessageQueue message_queue_;

  /**
   * @brief Mutex restricting access to messages.
   **/
#ifdef USE_RWLOCK
  mutable pthread_rwlock_t message_queue_rwlock_;
#endif
  mutable pthread_mutex_t message_queue_mutex_;

  pthread_cond_t message_queue_cond_;

  /**
   * @brief store events from comming device
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  EventQueue event_queue_;

  /**
   * @brief flag that indicates that thread is active
   * if it is false then threads exist main loop
   **/
  volatile bool all_thread_active_;

  typedef std::list<TransportManagerListener*> TransportManagerListenerList;
  /**
   * @brief listener that would be called when TM's event happened.
   **/
  TransportManagerListenerList transport_manager_listener_;

  /**
   * @brief ID of message queue processing thread
   **/
  pthread_t message_queue_thread_;

  /**
   * @brief Conditional event thread
   **/
  pthread_t event_queue_thread_;

  /**
   * @brief Condition variable to wake up event
   **/
  pthread_cond_t device_listener_thread_wakeup_;

  /**
   * @brief Mutex restricting access to events.
   **/
#ifdef USE_RWLOCK
  mutable pthread_rwlock_t event_queue_rwlock_;
#endif
  mutable pthread_mutex_t event_queue_mutex_;

  /**
   * @brief Flag that TM is initialized
   */
  bool is_initialized_;
#ifdef TIME_TESTER
  TMMetricObserver* metric_observer_;
#endif  // TIME_TESTER
 private:
  /**
   * @brief Structure that contains conversion functions (Device ID -> Device
   * Handle; Device Handle -> Device ID)
   */
  struct Handle2GUIDConverter {
    typedef std::vector<DeviceUID> ConversionTable;

    DeviceHandle UidToHandle(const DeviceUID& dev_uid) {
      bool is_new = true;
      return UidToHandle(dev_uid, is_new);
    }

    DeviceHandle UidToHandle(const DeviceUID& dev_uid, bool& is_new) {
      ConversionTable::iterator it = std::find(
          conversion_table_.begin(), conversion_table_.end(), dev_uid);
      if (it != conversion_table_.end()) {
        is_new = false;
        return std::distance(conversion_table_.begin(), it) +
               1;  // handle begin since 1 (one)
      }
      is_new = true;
      conversion_table_.push_back(dev_uid);
      return conversion_table_.size();  // handle begin since 1 (one)
    }

    DeviceUID HandleToUid(DeviceHandle handle) {
      if (handle == 0 || handle > conversion_table_.size()) {
        return DeviceUID();
      }
      return conversion_table_[handle - 1];  // handle begin since 1 (one)
    }

    ConversionTable conversion_table_;
  };

  /**
   * @brief Converter variable (Device ID -> Device Handle; Device Handle ->
   * Device ID)
   */
  Handle2GUIDConverter converter_;

  explicit TransportManagerImpl(const TransportManagerImpl&);
  int connection_id_counter_;
  std::vector<ConnectionInternal> connections_;
  std::map<DeviceUID, TransportAdapter*> device_to_adapter_map_;
  std::vector<TransportAdapter*> transport_adapters_;
  /** For keep listeners which were add TMImpl */
  std::map<TransportAdapter*, TransportAdapterListenerImpl*>
      transport_adapter_listeners_;

  typedef std::vector<std::pair<const TransportAdapter*, DeviceInfo> >
      DeviceList;
  DeviceList device_list_;
  void AddConnection(const ConnectionInternal& c);
  void RemoveConnection(int id);
  ConnectionInternal* GetConnection(const ConnectionUID& id);
  ConnectionInternal* GetConnection(const DeviceUID& device,
                                    const ApplicationHandle& application);

  void AddDataToContainer(
      ConnectionUID id,
      std::map<ConnectionUID, std::pair<unsigned int, unsigned char*> >&
          container,
      unsigned char* data, unsigned int data_size);
  bool GetFrameSize(unsigned char* data, unsigned int data_size,
                    unsigned int& frame_size);
  bool GetFrame(std::map<ConnectionUID,
                         std::pair<unsigned int, unsigned char*> >& container,
                ConnectionUID id, unsigned int frame_size,
                unsigned char** frame);

  void OnDeviceListUpdated(TransportAdapter* ta);
  static Connection convert(ConnectionInternal& p);
};
// class ;

}  // namespace transport_manager

#endif
