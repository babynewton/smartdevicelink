package com.ford.syncV4.proxy.rpc.enums;


public enum AppInterfaceUnregisteredReason {
    USER_EXIT,
    IGNITION_OFF,
    BLUETOOTH_OFF,
    USB_DISCONNECTED,
    REQUEST_WHILE_IN_NONE_HMI_LEVEL,
    TOO_MANY_REQUESTS,
    DRIVER_DISTRACTION_VIOLATION,
    LANGUAGE_CHANGE,
    MASTER_RESET,
    FACTORY_DEFAULTS,
    APP_UNAUTHORIZED;

    public static AppInterfaceUnregisteredReason valueForString(String value) {
        return valueOf(value);
    }
}
