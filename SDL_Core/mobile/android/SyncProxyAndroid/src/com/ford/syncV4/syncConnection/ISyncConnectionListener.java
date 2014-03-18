package com.ford.syncV4.syncConnection;

import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.session.Session;

public interface ISyncConnectionListener {
	public void onTransportDisconnected(String info);
	
	public void onTransportError(String info, Exception e);

    public void onHeartbeatTimedOut();
	
	public void onProtocolMessageReceived(ProtocolMessage msg);
	
	public void onProtocolSessionStarted(Session session, byte version, String correlationID);
	
	public void onProtocolServiceEnded(ServiceType serviceType,
                                       byte sessionID, String correlationID);
	
	public void onProtocolError(String info, Throwable e);

    public void onMobileNavAckReceived(int frameReceivedNumber);

    public void onProtocolServiceStarted(ServiceType serviceType, byte sessionID, byte version, String correlationID);

    public void onStartServiceNackReceived(ServiceType serviceType);
}
