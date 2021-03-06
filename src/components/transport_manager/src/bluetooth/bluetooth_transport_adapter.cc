/**
 * \file bluetooth_transport_adapter.cc
 * \brief BluetoothTransportAdapter class source file.
 *
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

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iomanip>
#include <set>
#include <bluetooth/bluetooth.h>

#include "resumption/last_state.h"

#include "transport_manager/bluetooth/bluetooth_transport_adapter.h"
#include "transport_manager/bluetooth/bluetooth_device_scanner.h"
#include "transport_manager/bluetooth/bluetooth_connection_factory.h"
#include "transport_manager/bluetooth/bluetooth_device.h"

namespace transport_manager {
namespace transport_adapter {

BluetoothTransportAdapter::~BluetoothTransportAdapter() {
}

BluetoothTransportAdapter::BluetoothTransportAdapter()
    : TransportAdapterImpl(new BluetoothDeviceScanner(this, true, 0),
                           new BluetoothConnectionFactory(this), 0) {
}

DeviceType BluetoothTransportAdapter::GetDeviceType() const {
  return "sdl-bluetooth";
}

void BluetoothTransportAdapter::Store() const {
  LOG4CXX_TRACE_ENTER(logger_);
  resumption::LastState::Dictionary bluetooth_adapter_dictionary;
  DeviceList device_ids = GetDeviceList();
  for (DeviceList::const_iterator i = device_ids.begin(); i != device_ids.end(); ++i) {
    DeviceUID device_id = *i;
    DeviceSptr device = FindDevice(device_id);
    if (!device) { // device could have been disconnected
      continue;
    }
    utils::SharedPtr<BluetoothDevice> bluetooth_device =
      DeviceSptr::static_pointer_cast<BluetoothDevice>(device);
    resumption::LastState::Dictionary device_dictionary;
    char address[18];
    ba2str(&bluetooth_device->address(), address);
    device_dictionary.AddItem("address", std::string(address));
    resumption::LastState::Dictionary applications_dictionary;
    ApplicationList app_ids = bluetooth_device->GetApplicationList();
    for (ApplicationList::const_iterator j = app_ids.begin(); j != app_ids.end(); ++j) {
      ApplicationHandle app_handle = *j;
      uint8_t rfcomm_channel;
      bluetooth_device->GetRfcommChannel(app_handle, &rfcomm_channel);
      resumption::LastState::Dictionary application_dictionary;
      application_dictionary.AddItem("rfcomm_channel", std::to_string(rfcomm_channel));
      applications_dictionary.AddSubitem(std::to_string(rfcomm_channel), application_dictionary);
    }
    device_dictionary.AddSubitem("applications", applications_dictionary);
    bluetooth_adapter_dictionary.AddSubitem(bluetooth_device->name(), device_dictionary);
  }
  resumption::LastState::instance()->dictionary.AddSubitem(
    "BluetoothAdapter", bluetooth_adapter_dictionary
  );
  LOG4CXX_TRACE_EXIT(logger_);
}

bool BluetoothTransportAdapter::Restore() {
  LOG4CXX_TRACE_ENTER(logger_);
  bool errors_occured = false;
  resumption::LastState::Dictionary bluetooth_adapter_dictionary =
    resumption::LastState::instance()->dictionary.SubitemAt("BluetoothAdapter");
  for (resumption::LastState::Dictionary::const_iterator i =
    bluetooth_adapter_dictionary.begin(); i != bluetooth_adapter_dictionary.end(); ++i) {
    std::string name = i->first;
    resumption::LastState::Dictionary device_dictionary = i->second;
    std::string address_record = device_dictionary.ItemAt("address");
    bdaddr_t address;
    str2ba(address_record.c_str(), &address);
    RfcommChannelVector rfcomm_channels;
    resumption::LastState::Dictionary applications_dictionary =
      device_dictionary.SubitemAt("applications");
    for (resumption::LastState::Dictionary::const_iterator j =
      applications_dictionary.begin(); j != applications_dictionary.end(); ++j) {
      resumption::LastState::Dictionary application_dictionary = j->second;
      std::string rfcomm_channel_record =
        application_dictionary.ItemAt("rfcomm_channel");
      uint8_t rfcomm_channel =
        static_cast<uint8_t>(atoi(rfcomm_channel_record.c_str()));
      rfcomm_channels.push_back(rfcomm_channel);
    }
    BluetoothDevice* bluetooth_device =
      new BluetoothDevice(address, name.c_str(), rfcomm_channels);
    DeviceSptr device(bluetooth_device);
    AddDevice(device);
    for (RfcommChannelVector::const_iterator j =
      rfcomm_channels.begin(); j != rfcomm_channels.end(); ++j) {
      ApplicationHandle app_handle = *j; // for Bluetooth device app_handle is just RFCOMM channel
      if (Error::OK != Connect(device->unique_device_id(), app_handle)) {
        errors_occured = true;
      }
    }
  }
  LOG4CXX_TRACE_EXIT(logger_);
  return !errors_occured;
}

}  // namespace transport_adapter
}  // namespace transport_manager

