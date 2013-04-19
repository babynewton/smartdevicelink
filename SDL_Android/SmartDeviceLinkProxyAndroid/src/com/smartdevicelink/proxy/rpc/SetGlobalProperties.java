//
// Copyright (c) 2013 Ford Motor Company
//
package com.smartdevicelink.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.smartdevicelink.proxy.RPCRequest;
import com.smartdevicelink.proxy.constants.Names;

public class SetGlobalProperties extends RPCRequest {

    public SetGlobalProperties() {
        super("SetGlobalProperties");
    }
    public SetGlobalProperties(Hashtable hash) {
        super(hash);
    }
    public Vector<TTSChunk> getHelpPrompt() {
    	if (parameters.get(Names.helpPrompt) instanceof Vector<?>) {
	        Vector<?> list = (Vector<?>)parameters.get(Names.helpPrompt);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof TTSChunk) {
	                return (Vector<TTSChunk>) list;
	            } else if (obj instanceof Hashtable) {
	                Vector<TTSChunk> newList = new Vector<TTSChunk>();
	                for (Object hashObj : list) {
	                    newList.add(new TTSChunk((Hashtable)hashObj));
	                }
	                return newList;
	            }
	        }
    	}
	    return null;
    }
    public void setHelpPrompt( Vector<TTSChunk> helpPrompt ) {
        if (helpPrompt != null) {
            parameters.put(Names.helpPrompt, helpPrompt );
        }
    }
    public Vector<TTSChunk> getTimeoutPrompt() {
        if (parameters.get(Names.timeoutPrompt) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)parameters.get(Names.timeoutPrompt);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof TTSChunk) {
	                return (Vector<TTSChunk>) list;
	            } else if (obj instanceof Hashtable) {
	                Vector<TTSChunk> newList = new Vector<TTSChunk>();
	                for (Object hashObj : list) {
	                    newList.add(new TTSChunk((Hashtable)hashObj));
	                }
	                return newList;
	            }
	        }
        }
        return null;
    }
    public void setTimeoutPrompt( Vector<TTSChunk> timeoutPrompt ) {
        if (timeoutPrompt != null) {
            parameters.put(Names.timeoutPrompt, timeoutPrompt );
        }
    }
}
