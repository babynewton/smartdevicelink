package com.ford.syncV4.integrationtest;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.marshal.IJsonRPCMarshaller;
import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.protocol.enums.MessageType;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.RPCNotification;
import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.SyncProxyALM;
import com.ford.syncV4.proxy.SyncProxyBase;
import com.ford.syncV4.proxy.converter.IRPCRequestConverterFactory;
import com.ford.syncV4.proxy.converter.SystemPutFileRPCRequestConverter;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALMTesting;
import com.ford.syncV4.proxy.rpc.OnSystemRequest;
import com.ford.syncV4.proxy.rpc.PutFileResponse;
import com.ford.syncV4.proxy.rpc.TestCommon;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.proxy.rpc.enums.RequestType;
import com.ford.syncV4.proxy.rpc.enums.Result;
import com.ford.syncV4.proxy.systemrequest.IOnSystemRequestHandler;
import com.ford.syncV4.proxy.systemrequest.ISystemRequestProxy;
import com.ford.syncV4.syncConnection.SyncConnection;
import com.ford.syncV4.util.TestConfig;

import org.hamcrest.core.IsNull;
import org.json.JSONException;
import org.json.JSONObject;
import org.mockito.ArgumentCaptor;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

import java.lang.reflect.Field;
import java.nio.charset.Charset;
import java.util.Arrays;
import java.util.List;
import java.util.Vector;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.core.Is.is;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.eq;
import static org.mockito.Matchers.notNull;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyZeroInteractions;
import static org.mockito.Mockito.when;

/**
 * This is an integration test for how the OnSystemRequest notification and the
 * PutFile request interact to provide system updates to SDL.
 *
 * Created by enikolsky on 2014-02-03.
 */
public class OnSystemRequest_PutFile_Test extends InstrumentationTestCase {
    private static final byte PROTOCOL_VERSION = (byte) 2;
    private static final int PUTFILE_FUNCTIONID = 32;
    private static final int ONSYSTEMREQUEST_FUNCTIONID = 32781;
    private static final String OFFSET = "offset";
    private static final String LENGTH = "length";
    private static final String SYNC_FILENAME = "syncFileName";
    private static final String SYSTEM_FILE = "systemFile";
    private static final String FILE_TYPE = "fileType";
    // TODO may cause test failure depending on device
    private static final int WAIT_TIMEOUT = 20;
    private IJsonRPCMarshaller marshaller;
    private IProxyListenerALMTesting proxyListenerMock;
    private WiProProtocol protocolMock;
    private SyncConnection connectionMock;
    private SyncProxyALM proxy;
    private int maxDataSize;

    @Override
    public void setUp() throws Exception {
        super.setUp();
        TestCommon.setupMocking(this);

        marshaller = new JsonRPCMarshaller();

        // we have to use the special IProxyListenerALMTesting here to allow to
        // send messages at any time
        proxyListenerMock = mock(IProxyListenerALMTesting.class);
        protocolMock = mock(WiProProtocol.class);
        connectionMock = createNewSyncConnectionMock();

        proxy = new SyncProxyALM(proxyListenerMock, null, "a", null, null,
                false, null, null, null, null, null, null, false, false, 2,
                null, connectionMock, new TestConfig());

        final SystemPutFileRPCRequestConverter converter =
                new SystemPutFileRPCRequestConverter();
        maxDataSize = 64;
        converter.setMaxDataSize(maxDataSize);
        IRPCRequestConverterFactory factoryMock =
                mock(IRPCRequestConverterFactory.class);
        when(factoryMock.getConverterForRequest(
                notNull(RPCRequest.class))).thenReturn(converter);
        proxy.setRpcRequestConverterFactory(factoryMock);
    }

    public void testOnSystemRequestRequestTypeHTTPShouldSendPartialPutFile()
            throws Exception {
        // fake data for PutFile
        final int extraDataSize = 10;
        final int dataSize = (maxDataSize * 2) + extraDataSize;
        final byte[] data = TestCommon.getRandomBytes(dataSize);

        final String filename = "fake";
        final List<String> urls = Arrays.asList("http://example.com/");
        final FileType fileType = FileType.GRAPHIC_PNG;

        IOnSystemRequestHandler handlerMock =
                mock(IOnSystemRequestHandler.class);
        doAnswer(new Answer() {
            @Override
            public Object answer(InvocationOnMock invocationOnMock)
                    throws Throwable {
                final ISystemRequestProxy proxy =
                        (ISystemRequestProxy) invocationOnMock.getArguments()[0];
                proxy.putSystemFile(filename, data, fileType);
                return null;
            }
        }).when(handlerMock)
          .onFilesDownloadRequest(notNull(ISystemRequestProxy.class), eq(urls),
                  eq(fileType));
        proxy.setOnSystemRequestHandler(handlerMock);

        // emulate incoming OnSystemRequest notification with HTTP
        OnSystemRequest onSysRq = new OnSystemRequest();
        onSysRq.setRequestType(RequestType.HTTP);
        onSysRq.setUrl(new Vector<String>(urls));
        onSysRq.setFileType(fileType);

        ProtocolMessage incomingOnSysRqPM0 =
                createNotificationProtocolMessage(onSysRq,
                        ONSYSTEMREQUEST_FUNCTIONID);
        emulateIncomingMessage(proxy, incomingOnSysRqPM0);

        // wait for processing
        Thread.sleep(WAIT_TIMEOUT);

        // expect the first part of PutFile to be sent
        ArgumentCaptor<ProtocolMessage> pmCaptor0 =
                ArgumentCaptor.forClass(ProtocolMessage.class);
        verify(connectionMock, times(1)).sendMessage(pmCaptor0.capture());

        // set another connection mock to be able to verify the second time below
        final SyncConnection connectionMock2 = createNewSyncConnectionMock();
        setSyncConnection(proxy, connectionMock2);

        final ProtocolMessage pm0 = pmCaptor0.getValue();
        assertThat(pm0.getFunctionID(), is(PUTFILE_FUNCTIONID));
        checkSystemPutFileJSON(pm0.getData(), 0, maxDataSize, filename,
                fileType);
        final byte[] data0 = Arrays.copyOfRange(data, 0, maxDataSize);
        assertThat(pm0.getBulkData(), is(data0));
        final int putFileRequestCorrID = pm0.getCorrID();

        // the listener should not be called for OnSystemRequest
        verifyZeroInteractions(proxyListenerMock);

        // emulate incoming PutFile response for first part
        PutFileResponse putFileResponse1 = new PutFileResponse();
        putFileResponse1.setResultCode(Result.SUCCESS);
        putFileResponse1.setCorrelationID(putFileRequestCorrID);

        ProtocolMessage incomingPutFileResponsePM1 =
                createResponseProtocolMessage(putFileResponse1,
                        putFileRequestCorrID, PUTFILE_FUNCTIONID);
        emulateIncomingMessage(proxy, incomingPutFileResponsePM1);

        // wait for processing
        Thread.sleep(WAIT_TIMEOUT);

        // expect the second part of PutFile to be sent
        ArgumentCaptor<ProtocolMessage> pmCaptor1 =
                ArgumentCaptor.forClass(ProtocolMessage.class);
        verify(connectionMock2, times(1)).sendMessage(pmCaptor1.capture());

        // set another connection mock to be able to verify the third time below
        final SyncConnection connectionMock3 = createNewSyncConnectionMock();
        setSyncConnection(proxy, connectionMock3);

        final ProtocolMessage pm1 = pmCaptor1.getValue();
        assertThat(pm1.getFunctionID(), is(PUTFILE_FUNCTIONID));
        checkSystemPutFileJSON(pm1.getData(), maxDataSize, maxDataSize,
                filename, fileType);
        final byte[] data1 =
                Arrays.copyOfRange(data, maxDataSize, maxDataSize * 2);
        assertThat(pm1.getBulkData(), is(data1));
        assertThat(pm1.getCorrID(), is(putFileRequestCorrID));

        // the listener should not be called for PutFile
        verifyZeroInteractions(proxyListenerMock);

        // emulate incoming PutFile response for second part
        PutFileResponse putFileResponse2 = new PutFileResponse();
        putFileResponse2.setResultCode(Result.SUCCESS);
        putFileResponse2.setCorrelationID(putFileRequestCorrID);

        ProtocolMessage incomingPutFileResponsePM2 =
                createResponseProtocolMessage(putFileResponse2,
                        putFileRequestCorrID, PUTFILE_FUNCTIONID);
        emulateIncomingMessage(proxy, incomingPutFileResponsePM2);

        // wait for processing
        Thread.sleep(WAIT_TIMEOUT);

        // expect the third part of PutFile to be sent
        ArgumentCaptor<ProtocolMessage> pmCaptor2 =
                ArgumentCaptor.forClass(ProtocolMessage.class);
        verify(connectionMock3, times(1)).sendMessage(pmCaptor2.capture());

        final ProtocolMessage pm2 = pmCaptor2.getValue();
        assertThat(pm2.getFunctionID(), is(PUTFILE_FUNCTIONID));
        checkSystemPutFileJSON(pm2.getData(), maxDataSize * 2, extraDataSize,
                filename, fileType);
        final byte[] data2 = Arrays.copyOfRange(data, maxDataSize * 2,
                (maxDataSize * 2) + extraDataSize);
        assertThat(pm2.getBulkData(), is(data2));
        assertThat(pm2.getCorrID(), is(putFileRequestCorrID));

        // the listener should not be called for PutFile
        verifyZeroInteractions(proxyListenerMock);

        // emulate incoming PutFile response for third part
        PutFileResponse putFileResponse3 = new PutFileResponse();
        putFileResponse3.setResultCode(Result.SUCCESS);
        putFileResponse3.setCorrelationID(putFileRequestCorrID);

        ProtocolMessage incomingPutFileResponsePM3 =
                createResponseProtocolMessage(putFileResponse3,
                        putFileRequestCorrID, PUTFILE_FUNCTIONID);
        emulateIncomingMessage(proxy, incomingPutFileResponsePM3);

        // wait for processing
        Thread.sleep(WAIT_TIMEOUT);

        // the listener should not be called for PutFile or OnSystemRequest
        verify(proxyListenerMock, never()).onPutFileResponse(
                any(PutFileResponse.class));
        verify(proxyListenerMock, never()).onOnSystemRequest(
                any(OnSystemRequest.class));

        // phew, done
    }

    public void testSystemPutFileShouldStopOnErrorResponse() throws Exception {
        // fake data for PutFile
        final int extraDataSize = 10;
        final int dataSize = (maxDataSize * 2) + extraDataSize;
        final byte[] data = TestCommon.getRandomBytes(dataSize);

        final String filename = "fake";
        final List<String> urls = Arrays.asList("http://example.com/");
        final FileType fileType = FileType.GRAPHIC_PNG;

        IOnSystemRequestHandler handlerMock =
                mock(IOnSystemRequestHandler.class);
        doAnswer(new Answer() {
            @Override
            public Object answer(InvocationOnMock invocationOnMock)
                    throws Throwable {
                final ISystemRequestProxy proxy =
                        (ISystemRequestProxy) invocationOnMock.getArguments()[0];
                proxy.putSystemFile(filename, data, fileType);
                return null;
            }
        }).when(handlerMock)
          .onFilesDownloadRequest(notNull(ISystemRequestProxy.class), eq(urls),
                  eq(fileType));
        proxy.setOnSystemRequestHandler(handlerMock);

        // emulate incoming OnSystemRequest notification with HTTP
        OnSystemRequest onSysRq = new OnSystemRequest();
        onSysRq.setRequestType(RequestType.HTTP);
        onSysRq.setUrl(new Vector<String>(urls));
        onSysRq.setFileType(fileType);

        ProtocolMessage incomingOnSysRqPM0 =
                createNotificationProtocolMessage(onSysRq,
                        ONSYSTEMREQUEST_FUNCTIONID);
        emulateIncomingMessage(proxy, incomingOnSysRqPM0);

        // wait for processing
        Thread.sleep(WAIT_TIMEOUT);

        // expect the first part of PutFile to be sent
        ArgumentCaptor<ProtocolMessage> pmCaptor0 =
                ArgumentCaptor.forClass(ProtocolMessage.class);
        verify(connectionMock, times(1)).sendMessage(pmCaptor0.capture());

        // set another connection mock to be able to verify the second time below
        final SyncConnection connectionMock2 = createNewSyncConnectionMock();
        setSyncConnection(proxy, connectionMock2);

        final ProtocolMessage pm0 = pmCaptor0.getValue();
        assertThat(pm0.getFunctionID(), is(PUTFILE_FUNCTIONID));
        checkSystemPutFileJSON(pm0.getData(), 0, maxDataSize, filename,
                fileType);
        final byte[] data0 = Arrays.copyOfRange(data, 0, maxDataSize);
        assertThat(pm0.getBulkData(), is(data0));
        final int putFileRequestCorrID = pm0.getCorrID();

        // the listener should not be called for OnSystemRequest
        verifyZeroInteractions(proxyListenerMock);

        // emulate incoming PutFile error response for first part
        final Result resultCode1 = Result.INVALID_DATA;
        PutFileResponse putFileResponse1 = new PutFileResponse();
        putFileResponse1.setResultCode(resultCode1);
        putFileResponse1.setCorrelationID(putFileRequestCorrID);

        ProtocolMessage incomingPutFileResponsePM1 =
                createResponseProtocolMessage(putFileResponse1,
                        putFileRequestCorrID, PUTFILE_FUNCTIONID);
        emulateIncomingMessage(proxy, incomingPutFileResponsePM1);

        // wait for processing
        Thread.sleep(WAIT_TIMEOUT * 2);

        // expect the second part of PutFile not to be sent
        verify(connectionMock2, never()).sendMessage(
                any(ProtocolMessage.class));

        // the listener should not be called for PutFile or OnSystemRequest
        verify(proxyListenerMock, never()).onPutFileResponse(
                any(PutFileResponse.class));
        verify(proxyListenerMock, never()).onOnSystemRequest(
                any(OnSystemRequest.class));

        // phew, done
    }

    public void testOnSystemRequestRequestTypeFileResumeShouldSendPartialPutFile()
            throws Exception {
        // fake data for PutFile
        final int extraDataSize = 30;
        final int dataSize = (maxDataSize * 2) + extraDataSize;
        final byte[] data = TestCommon.getRandomBytes(dataSize);

        final String filename = "fake";
        final List<String> filenames = Arrays.asList(filename);
        final FileType fileType = FileType.GRAPHIC_PNG;
        final int offset = 4000;

        IOnSystemRequestHandler handlerMock =
                mock(IOnSystemRequestHandler.class);
        doAnswer(new Answer() {
            @Override
            public Object answer(InvocationOnMock invocationOnMock)
                    throws Throwable {
                final ISystemRequestProxy proxy =
                        (ISystemRequestProxy) invocationOnMock.getArguments()[0];
                proxy.putSystemFile(filename, data, offset, fileType);
                return null;
            }
        }).when(handlerMock)
          .onFileResumeRequest(notNull(ISystemRequestProxy.class), eq(filename),
                  eq(offset), eq(dataSize), eq(fileType));
        proxy.setOnSystemRequestHandler(handlerMock);

        // emulate incoming OnSystemRequest notification with FILE_RESUME
        OnSystemRequest onSysRq = new OnSystemRequest();
        onSysRq.setRequestType(RequestType.FILE_RESUME);
        onSysRq.setUrl(new Vector<String>(filenames));
        onSysRq.setFileType(fileType);
        onSysRq.setOffset(offset);
        onSysRq.setLength(dataSize);

        ProtocolMessage incomingOnSysRqPM0 =
                createNotificationProtocolMessage(onSysRq,
                        ONSYSTEMREQUEST_FUNCTIONID);
        emulateIncomingMessage(proxy, incomingOnSysRqPM0);

        // wait for processing
        Thread.sleep(WAIT_TIMEOUT);

        // expect the first part of PutFile to be sent
        ArgumentCaptor<ProtocolMessage> pmCaptor0 =
                ArgumentCaptor.forClass(ProtocolMessage.class);
        verify(connectionMock, times(1)).sendMessage(pmCaptor0.capture());

        // set another connection mock to be able to verify the second time below
        final SyncConnection connectionMock2 = createNewSyncConnectionMock();
        setSyncConnection(proxy, connectionMock2);

        final ProtocolMessage pm0 = pmCaptor0.getValue();
        assertThat(pm0.getFunctionID(), is(PUTFILE_FUNCTIONID));
        checkSystemPutFileJSON(pm0.getData(), offset, maxDataSize, filename,
                fileType);
        final byte[] data0 = Arrays.copyOfRange(data, 0, maxDataSize);
        assertThat(pm0.getBulkData(), is(data0));
        final int putFileRequestCorrID = pm0.getCorrID();

        // the listener should not be called for OnSystemRequest
        verify(proxyListenerMock, never()).onOnSystemRequest(
                any(OnSystemRequest.class));
        verifyZeroInteractions(proxyListenerMock);

        // emulate incoming PutFile response for first part
        PutFileResponse putFileResponse1 = new PutFileResponse();
        putFileResponse1.setResultCode(Result.SUCCESS);
        putFileResponse1.setCorrelationID(putFileRequestCorrID);

        ProtocolMessage incomingPutFileResponsePM1 =
                createResponseProtocolMessage(putFileResponse1,
                        putFileRequestCorrID, PUTFILE_FUNCTIONID);
        emulateIncomingMessage(proxy, incomingPutFileResponsePM1);

        // wait for processing
        Thread.sleep(WAIT_TIMEOUT);

        // expect the second part of PutFile to be sent
        ArgumentCaptor<ProtocolMessage> pmCaptor1 =
                ArgumentCaptor.forClass(ProtocolMessage.class);
        verify(connectionMock2, times(1)).sendMessage(pmCaptor1.capture());

        // set another connection mock to be able to verify the third time below
        final SyncConnection connectionMock3 = createNewSyncConnectionMock();
        setSyncConnection(proxy, connectionMock3);

        final ProtocolMessage pm1 = pmCaptor1.getValue();
        assertThat(pm1.getFunctionID(), is(PUTFILE_FUNCTIONID));
        checkSystemPutFileJSON(pm1.getData(), offset + maxDataSize, maxDataSize,
                filename, fileType);
        final byte[] data1 =
                Arrays.copyOfRange(data, maxDataSize, maxDataSize * 2);
        assertThat(pm1.getBulkData(), is(data1));
        assertThat(pm1.getCorrID(), is(putFileRequestCorrID));

        // the listener should not be called for PutFile
        verifyZeroInteractions(proxyListenerMock);

        // emulate incoming PutFile response for second part
        PutFileResponse putFileResponse2 = new PutFileResponse();
        putFileResponse2.setResultCode(Result.SUCCESS);
        putFileResponse2.setCorrelationID(putFileRequestCorrID);

        ProtocolMessage incomingPutFileResponsePM2 =
                createResponseProtocolMessage(putFileResponse2,
                        putFileRequestCorrID, PUTFILE_FUNCTIONID);
        emulateIncomingMessage(proxy, incomingPutFileResponsePM2);

        // wait for processing
        Thread.sleep(WAIT_TIMEOUT);

        // expect the third part of PutFile to be sent
        ArgumentCaptor<ProtocolMessage> pmCaptor2 =
                ArgumentCaptor.forClass(ProtocolMessage.class);
        verify(connectionMock3, times(1)).sendMessage(pmCaptor2.capture());

        final ProtocolMessage pm2 = pmCaptor2.getValue();
        assertThat(pm2.getFunctionID(), is(PUTFILE_FUNCTIONID));
        checkSystemPutFileJSON(pm2.getData(), offset + (maxDataSize * 2),
                extraDataSize, filename, fileType);
        final byte[] data2 = Arrays.copyOfRange(data, maxDataSize * 2,
                (maxDataSize * 2) + extraDataSize);
        assertThat(pm2.getBulkData(), is(data2));
        assertThat(pm2.getCorrID(), is(putFileRequestCorrID));

        // the listener should not be called for PutFile
        verifyZeroInteractions(proxyListenerMock);

        // emulate incoming PutFile response for third part
        PutFileResponse putFileResponse3 = new PutFileResponse();
        putFileResponse3.setResultCode(Result.SUCCESS);
        putFileResponse3.setCorrelationID(putFileRequestCorrID);

        ProtocolMessage incomingPutFileResponsePM3 =
                createResponseProtocolMessage(putFileResponse3,
                        putFileRequestCorrID, PUTFILE_FUNCTIONID);
        emulateIncomingMessage(proxy, incomingPutFileResponsePM3);

        // wait for processing
        Thread.sleep(WAIT_TIMEOUT);

        // the listener should not be called for PutFile or OnSystemRequest
        verify(proxyListenerMock, never()).onPutFileResponse(
                any(PutFileResponse.class));
        verify(proxyListenerMock, never()).onOnSystemRequest(
                any(OnSystemRequest.class));

        // phew, done
    }

    // TODO check the rest is not sent after reconnect

    private SyncConnection createNewSyncConnectionMock() {
        SyncConnection connectionMock2 = mock(SyncConnection.class);
        when(connectionMock2.getIsConnected()).thenReturn(true);
        when(connectionMock2.getWiProProtocol()).thenReturn(protocolMock);
        return connectionMock2;
    }

    private ProtocolMessage createResponseProtocolMessage(RPCResponse response,
                                                          int correlationID,
                                                          int functionID) {
        ProtocolMessage incomingPM0 = new ProtocolMessage();
        incomingPM0.setVersion(PROTOCOL_VERSION);
        byte[] msgBytes = marshaller.marshall(response, PROTOCOL_VERSION);
        incomingPM0.setData(msgBytes);
        incomingPM0.setJsonSize(msgBytes.length);
        incomingPM0.setMessageType(MessageType.RPC);
        incomingPM0.setSessionType(ServiceType.RPC);
        incomingPM0.setFunctionID(functionID);
        incomingPM0.setRPCType(ProtocolMessage.RPCTYPE_RESPONSE);
        incomingPM0.setCorrID(correlationID);
        return incomingPM0;
    }

    private ProtocolMessage createNotificationProtocolMessage(
            RPCNotification notification, int functionID) {
        ProtocolMessage incomingPM0 = new ProtocolMessage();
        incomingPM0.setVersion(PROTOCOL_VERSION);
        byte[] msgBytes = marshaller.marshall(notification, PROTOCOL_VERSION);
        incomingPM0.setData(msgBytes);
        incomingPM0.setJsonSize(msgBytes.length);
        incomingPM0.setMessageType(MessageType.RPC);
        incomingPM0.setSessionType(ServiceType.RPC);
        incomingPM0.setFunctionID(functionID);
        incomingPM0.setRPCType(ProtocolMessage.RPCTYPE_NOTIFICATION);
        return incomingPM0;
    }

    private void emulateIncomingMessage(SyncProxyALM proxy, ProtocolMessage pm)
            throws NoSuchFieldException, IllegalAccessException {
        final Field interfaceBroker =
                SyncProxyBase.class.getDeclaredField("_interfaceBroker");
        interfaceBroker.setAccessible(true);
        SyncProxyBase.SyncInterfaceBroker broker =
                (SyncProxyBase.SyncInterfaceBroker) interfaceBroker.get(proxy);
        broker.onProtocolMessageReceived(pm);
    }

    private void setSyncConnection(SyncProxyALM proxy,
                                   SyncConnection connection)
            throws NoSuchFieldException, IllegalAccessException {
        final Field syncConnection =
                SyncProxyBase.class.getDeclaredField("mSyncConnection");
        syncConnection.setAccessible(true);
        syncConnection.set(proxy, connection);
    }

    private void checkSystemPutFileJSON(byte[] data, int offset, int length,
                                        String filename, FileType fileType)
            throws JSONException {
        assertThat("JSON data must not be null", data, IsNull.notNullValue());

        JSONObject jsonObject =
                new JSONObject(new String(data, Charset.defaultCharset()));
        assertThat("offset doesn't match", jsonObject.getInt(OFFSET),
                is(offset));
        assertThat("length doesn't match", jsonObject.getInt(LENGTH),
                is(length));
        assertThat("filename must be set", jsonObject.getString(SYNC_FILENAME),
                is(filename));
        assertThat("systemFile must be true",
                jsonObject.getBoolean(SYSTEM_FILE), is(true));
        assertThat("fileType must be set", jsonObject.getString(FILE_TYPE),
                is(fileType.toString()));
    }
}
