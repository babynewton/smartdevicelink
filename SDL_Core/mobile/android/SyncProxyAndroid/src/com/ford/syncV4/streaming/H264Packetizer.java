package com.ford.syncV4.streaming;

import android.util.Log;

import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.enums.ServiceType;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.util.Arrays;

public class H264Packetizer extends AbstractPacketizer implements Runnable {
    public final static String TAG = "H264Packetizer";
    private byte[] tail = null;

    private Thread thread = null;

    private ByteBuffer byteBuffer = ByteBuffer.allocate(MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE);
    private byte[] dataBuffer = new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE];

    public Thread getThread() {
        return thread;
    }

    public H264Packetizer(IStreamListener streamListener, InputStream is, byte rpcSessionID, ServiceType serviceType) throws IOException {
        super(streamListener, is, rpcSessionID);
        _serviceType = serviceType;
    }

    public void start() throws IOException {
        if (thread == null) {
            thread = new Thread(this);
            thread.start();
        }
    }

    public void stop() {
        try {
            if (is != null) {
                is.close();
            }
        } catch (IOException ignore) {
        }
        if (thread != null) {
            thread.interrupt();
            thread = null;
        }
    }

    public void run() {
        try {
            while (!Thread.interrupted()) {
                try {
                    doDataReading();

                } catch (IllegalArgumentException e) {
                    Log.e(TAG, e.toString());
                    break;
                    //TODO - this NPE is really last hope to save app form crash. We must get sure never have it.
                } catch (NullPointerException e) {
                    Log.e(TAG, e.toString());
                    break;
                }
            }
        } catch (IOException e) {
            Log.e(TAG, "H264 error",e);
        }
    }

    public synchronized void doDataReading() throws IOException, IllegalArgumentException {
        byte[] frameData = readFrameData(byteBuffer, dataBuffer);
        if (frameData != null && frameData.length > 0) {
            createProtocolMessage(frameData);
        }
    }

    public ProtocolMessage createProtocolMessage(byte[] frameData) {
        ProtocolMessage pm = new ProtocolMessage();
        pm.setSessionID(_rpcSessionID);
        pm.setSessionType(_serviceType);
        pm.setFunctionID(0);
        pm.setCorrID(0);
        pm.setData(frameData, frameData.length);

        _streamListener.sendH264(pm);
        return pm;
    }

    public byte[] readFrameData(ByteBuffer buffer, byte[] data) throws IOException, IllegalArgumentException {
        if (tail != null) {
            buffer.put(tail);
        }
        do {
            MobileNaviDataFrame frame = createFramePayload(data);
            if (frame.getType() == MobileNaviDataFrameType.END_OS_SESSION_TYPE) {
                byte[] result = Arrays.copyOf(buffer.array(), buffer.position());
                buffer.clear();
                return result;
            }
            if (frame.getData().length >= buffer.remaining()) {
                tail = Arrays.copyOfRange(frame.getData(), buffer.remaining(), frame.getData().length);
                buffer.put(frame.getData(), 0, buffer.remaining());
            } else {
                buffer.put(frame.getData(), 0, frame.getData().length);
            }
        } while (buffer.remaining() > 0);
        byte[] result = buffer.array();
        buffer.clear();
        return result;
    }

    private MobileNaviDataFrame createFramePayload(byte[] data) throws IOException, IllegalArgumentException {
        int length = readDataFromStream(data);
        if (length == -1) {
            return MobileNaviDataFrame.createEndOfServiceFrame();
        } else {
            MobileNaviDataFrame frame = null;
            if (data.length == length) {
                frame = new MobileNaviDataFrame(data.clone());
            } else {
                frame = new MobileNaviDataFrame(Arrays.copyOf(data, length));
            }
            return frame;
        }
    }

    private int readDataFromStream(byte[] data) throws IOException {
        checkPreconditions(data);
        int res = is.read(data);
        return res;
    }

    private void checkPreconditions(byte[] data) {
        if (is == null) {
            throw new IllegalArgumentException("Input stream is null");
        }
        if (data == null) {
            throw new IllegalArgumentException("data is null");
        }
    }
}
