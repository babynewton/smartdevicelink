package com.ford.syncV4.protocol;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.session.Session;

import junit.framework.TestCase;

/**
 * Created by u on 2013-09-30.
 */
public class WiProProtocolUSBTest extends TestCase {
    private boolean requiredMethodCalled;

    public void testUnregisterAppInterfaceResponseCallsProtocolListener() {
        final IProtocolListener protocolListener = new IProtocolListener() {
            @Override
            public void onProtocolMessageBytesToSend(byte[] msgBytes,
                                                     int offset, int length) {
            }

            @Override
            public void onProtocolMessageReceived(ProtocolMessage msg) {
            }

            @Override
            public void onProtocolSessionStarted(Session session,
                                                 byte version,
                                                 String correlationID) {
            }

            @Override
            public void onProtocolServiceEnded(ServiceType sessionType,
                                               byte sessionID,
                                               String correlationID) {
            }

            @Override
            public void onProtocolHeartbeatACK() {
            }

            @Override
            public void onResetHeartbeat() {

            }

            @Override
            public void onProtocolError(String info, Exception e) {
            }

            @Override
            public void onMobileNavAckReceived(int frameReceivedNumber) {
            }

            @Override
            public void onProtocolAppUnregistered() {
                requiredMethodCalled = true;
            }

            @Override
            public void onProtocolServiceStarted(ServiceType serviceType, byte sessionID, byte version, String correlationID) {

            }

            @Override
            public void onStartServiceNackReceived(ServiceType serviceType) {

            }
        };

        requiredMethodCalled = false;
        final WiProProtocol protocol = new WiProProtocol(protocolListener);
        protocol._version = 2;

        byte[] unregisterAppInterfaceResponse = new byte[]{
                // first bytes
                0x21, 0x07, 0x00, 0x01,
                // data size
                0x00, 0x00, 0x00, 14,
                // message ID
                0x00, 0x00, 0x00, 0x01,
                // RPC type and function ID
                0x10, 0x00, 0x00, 0x02,
                // correlation ID
                0x00, 0x00, 0x00, 0x01,
                // JSON size
                0x00, 0x00, 0x00, 0x02,
                // JSON
                '{', '}'
        };

        protocol.HandleReceivedBytes(unregisterAppInterfaceResponse,
                unregisterAppInterfaceResponse.length);
        assertTrue("onProtocolAppUnregistered() method isn't called",
                requiredMethodCalled);
    }

    public void testUnregisterAppInterfaceRequestDoesntCallProtocolListener() {
        final IProtocolListener protocolListener = new IProtocolListener() {
            @Override
            public void onProtocolMessageBytesToSend(byte[] msgBytes,
                                                     int offset, int length) {
            }

            @Override
            public void onProtocolMessageReceived(ProtocolMessage msg) {
            }

            @Override
            public void onProtocolSessionStarted(Session session,
                                                 byte version,
                                                 String correlationID) {
            }

            @Override
            public void onProtocolServiceEnded(ServiceType sessionType,
                                               byte sessionID,
                                               String correlationID) {
            }

            @Override
            public void onProtocolHeartbeatACK() {
            }

            @Override
            public void onResetHeartbeat() {

            }

            @Override
            public void onProtocolError(String info, Exception e) {
            }

            @Override
            public void onMobileNavAckReceived(int frameReceivedNumber) {
            }

            @Override
            public void onProtocolAppUnregistered() {
                assertFalse(
                        "Incoming UnregisterAppInterface request shouldn't " +
                                "trigger the method", true);
            }

            @Override
            public void onProtocolServiceStarted(ServiceType serviceType, byte sessionID, byte version, String correlationID) {

            }

            @Override
            public void onStartServiceNackReceived(ServiceType serviceType) {

            }
        };

        final WiProProtocol protocol = new WiProProtocol(protocolListener);
        protocol._version = 2;

        byte[] unregisterAppInterfaceRequest = new byte[]{
                // first bytes
                0x21, 0x07, 0x00, 0x01,
                // data size
                0x00, 0x00, 0x00, 14,
                // message ID
                0x00, 0x00, 0x00, 0x01,
                // RPC type and function ID
                0x00, 0x00, 0x00, 0x02,
                // correlation ID
                0x00, 0x00, 0x00, 0x01,
                // JSON size
                0x00, 0x00, 0x00, 0x02,
                // JSON
                '{', '}'
        };

        protocol.HandleReceivedBytes(unregisterAppInterfaceRequest,
                unregisterAppInterfaceRequest.length);
    }
}
