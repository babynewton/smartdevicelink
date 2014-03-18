package com.ford.syncV4.proxy.session;

import com.ford.syncV4.protocol.AbstractProtocol;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.session.Session;

/**
 * Created by Andrew Batutin on 8/20/13.
 * <p/>
 * Abstraction Over Mobile Navigation Session Concept
 */

public class MobileNavSession {

    private AbstractProtocol _protocol;

    private MobileNavSession() {
    }

    public MobileNavSession(AbstractProtocol protocol){
        _protocol = protocol;
    }

    public void startSession(Session session) {
        _protocol.StartProtocolService(ServiceType.Mobile_Nav, session);
    }
}
