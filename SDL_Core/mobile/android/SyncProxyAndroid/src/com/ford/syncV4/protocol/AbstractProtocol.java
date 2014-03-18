package com.ford.syncV4.protocol;

import android.os.Environment;
import android.util.Log;

import com.ford.syncV4.protocol.WiProProtocol.MessageFrameAssembler;
import com.ford.syncV4.protocol.enums.FrameType;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.session.Session;
import com.ford.syncV4.streaming.AbstractPacketizer;
import com.ford.syncV4.trace.SyncTrace;
import com.ford.syncV4.trace.enums.InterfaceActivityDirection;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;

public abstract class AbstractProtocol {
    private static final String SYNC_LIB_TRACE_KEY = "42baba60-eb57-11df-98cf-0800200c9a66";
    protected IProtocolListener _protocolListener = null;
    //protected IProtocolListener ProtocolListener() { return _protocolListener; }
    // Lock to ensure all frames are sent uninterupted
    private Object _frameLock = new Object();
    private static File audioFile;
    private static File videoFile;
    private static FileOutputStream audioOutputFileStream;
    private static FileOutputStream videoOutputFileStream;

    // Caller must provide a non-null IProtocolListener interface reference.
    public AbstractProtocol(IProtocolListener protocolListener) {
        if (protocolListener == null) {
            throw new IllegalArgumentException("Provided protocol listener interface reference is null");
        } // end-if
        _protocolListener = protocolListener;
    }// end-ctor


    // This method receives raw bytes as they arrive from transport.  Those bytes
    // are then collected by the protocol and assembled into complete messages and
    // handled internally by the protocol or propagated to the protocol listener.

    public abstract void HandleReceivedBytes(byte[] receivedBytes, int length);

    // This method receives a protocol message (e.g. RPC, BULK, etc.) and processes
    // it for transmission over the transport.  The results of this processing will
    // be sent to the onProtocolMessageBytesToSend() method on protocol listener
    // interface.  Note that the ProtocolMessage itself contains information
    // about the type of message (e.g. RPC, BULK, etc.) and the protocol currentSession
    // over which to send the message, etc.
    public abstract void SendMessage(ProtocolMessage msg);

    /**
     * This method starts a protocol currentSession. A corresponding call to the protocol
     * listener onProtocolSessionStarted() method will be made when the protocol
     * currentSession has been established.
     *
     * @param sessionId ID of the current active session
     */
    public abstract void StartProtocolSession(byte sessionId);

    public abstract void StartProtocolService(ServiceType serviceType, Session session);

    // This method ends a protocol currentSession.  A corresponding call to the protocol
    // listener onProtocolServiceEnded() method will be made when the protocol
    // currentSession has ended.
    public abstract void EndProtocolService(ServiceType serviceType, byte sessionID);

    // TODO REMOVE
    // This method sets the interval at which heartbeat protocol messages will be
    // sent to SYNC.
    public abstract void SetHeartbeatSendInterval(int heartbeatSendInterval_ms);

    // This method sets the interval at which heartbeat protocol messages are
    // expected to be received from SYNC.
    public abstract void SetHeartbeatReceiveInterval(int heartbeatReceiveInterval_ms);

    // This method is called whenever the protocol receives a complete frame
    protected void handleProtocolFrameReceived(ProtocolFrameHeader header, byte[] data, MessageFrameAssembler assembler) {
        SyncTrace.logProtocolEvent(InterfaceActivityDirection.Receive, header, data,
                0, data.length, SYNC_LIB_TRACE_KEY);

        assembler.handleFrame(header, data);
    }

    // This method is called whenever a protocol has an entire frame to send
    protected void handleProtocolFrameToSend(ProtocolFrameHeader header, byte[] data, int offset, int length) {
        SyncTrace.logProtocolEvent(InterfaceActivityDirection.Transmit, header, data,
                offset, length, SYNC_LIB_TRACE_KEY);
        resetHeartbeat();
        synchronized (_frameLock) {
            byte[] frameHeader = header.assembleHeaderBytes();
            handleProtocolMessageBytesToSend(frameHeader, 0, frameHeader.length);

            if (data != null) {
                handleProtocolMessageBytesToSend(data, offset, length);
            }

        } // end-if
    }

    private synchronized void resetHeartbeat() {
        if (_protocolListener != null) {
            _protocolListener.onResetHeartbeat();
        }
    }

    private void logMobileNaviMessages(ProtocolFrameHeader header, byte[] data) {
        if (header.getServiceType().equals(ServiceType.Audio_Service)) {
            Log.d("AUDIO SERVCIE", "ProtocolFrameHeader: " + header.toString());
            if (data != null && data.length > 0) {
                Log.d("AUDIO SERVCIE", "Hex Data frame: " + AbstractPacketizer.printBuffer(data, 0, data.length));
            }
        }
    }

    private void initVideoDumpStream() {
        String filename = "ford_video.txt";
        if (videoFile == null) {
            videoFile = new File(Environment.getExternalStorageDirectory(), filename);
        }
        if (videoOutputFileStream == null) {
            try {
                videoOutputFileStream = new FileOutputStream(videoFile);
            } catch (FileNotFoundException e) {
                // handle exception
            }
        }
    }

    private void initAudioDumpStream() {
        String filename = "ford_audio.txt";
        if (audioFile == null) {
            audioFile = new File(Environment.getExternalStorageDirectory(), filename);
        }
        if (audioOutputFileStream == null) {
            try {
                audioOutputFileStream = new FileOutputStream(audioFile);
            } catch (FileNotFoundException e) {
                // handle exception
            }
        }
    }

    private void writeToSdCardAudioFile(ProtocolFrameHeader header, byte[] data) {
        if (header.getServiceType().equals(ServiceType.Audio_Service)) {
            if (header.getFrameType().equals(FrameType.Single)) {
               /* try {
                    if (audioOutputFileStream != null) {
                        audioOutputFileStream.write(data);
                    }
                } catch (IOException e) {
                    Log.w("SyncProxyTester", e.toString());
                }*/
                // Log.d("ford_audio.txt","audio: " + new String(data));
            } else {
                Log.w("SyncProxyTester", "wrong frame type for video streaming");
            }
        }
    }

    private void writeToSdCardVideoFile(ProtocolFrameHeader header, byte[] data) {
        if (header.getServiceType().equals(ServiceType.Mobile_Nav)) {
            if (header.getFrameType().equals(FrameType.Single)) {
               /* try {
                    if (videoOutputFileStream != null) {
                        videoOutputFileStream.write(data);
                    }
                } catch (IOException e) {
                    Log.w("SyncProxyTester", e.toString());
                }*/
                // Log.d("ford_video.txt","video: " + new String(data));
            } else {
                Log.w("SyncProxyTester", "wrong frame type for video streaming");
            }
        }
    }

    // This method handles protocol message bytes that are ready to send.
    // A callback is sent to the protocol listener.
    protected void handleProtocolMessageBytesToSend(byte[] bytesToSend,
                                                    int offset, int length) {
        _protocolListener.onProtocolMessageBytesToSend(bytesToSend, offset, length);
    }

    // This method handles received protocol messages.
    protected void handleProtocolMessageReceived(ProtocolMessage message) {
        _protocolListener.onProtocolMessageReceived(message);
    }

    // This method handles the end of a protocol currentSession. A callback is
    // sent to the protocol listener.
    protected void handleProtocolServiceEnded(ServiceType serviceType,
                                              byte sessionID, String correlationID) {
        _protocolListener.onProtocolServiceEnded(serviceType, sessionID, correlationID);
    }

    // This method handles the startup of a protocol currentSession. A callback is sent
    // to the protocol listener.
    protected void handleProtocolSessionStarted(ServiceType serviceType,
                                                byte sessionID, byte version, String correlationID) {
        Session session = Session.createSession(serviceType, sessionID);
        _protocolListener.onProtocolSessionStarted(session, version, correlationID);
    }

    protected void handleProtocolServiceStarted(ServiceType serviceType,
                                                byte sessionID, byte version, String correlationID) {
        if (serviceType.equals(ServiceType.RPC)) {
            throw new IllegalArgumentException("Can't create RPC service without creating currentSession. serviceType" + serviceType + ";sessionID " + sessionID);
        }
        if (sessionID == 0) {
            throw new IllegalArgumentException("Can't create service with id 0. serviceType" + serviceType + ";sessionID " + sessionID);
        }
        _protocolListener.onProtocolServiceStarted(serviceType, sessionID, version, correlationID);
    }

    // This method handles protocol errors. A callback is sent to the protocol
    // listener.
    protected void handleProtocolError(String string, Exception ex) {
        _protocolListener.onProtocolError(string, ex);
    }

    protected void handleAppUnregistered() {
        _protocolListener.onProtocolAppUnregistered();
    }

    protected void handleProtocolHeartbeatACK() {
        _protocolListener.onProtocolHeartbeatACK();
    }
} // end-class
