//
// Copyright (c) 2013 Ford Motor Company
//
package com.smartdevicelink.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.smartdevicelink.proxy.RPCRequest;
import com.smartdevicelink.proxy.constants.Names;
import com.smartdevicelink.proxy.rpc.enums.InteractionMode;
import com.smartdevicelink.util.DebugTool;

public class PerformInteraction extends RPCRequest {

    public PerformInteraction() {
        super("PerformInteraction");
    }
    public PerformInteraction(Hashtable hash) {
        super(hash);
    }
    public String getInitialText() {
        return (String) parameters.get( Names.initialText );
    }
    public void setInitialText( String initialText ) {
        if (initialText != null) {
            parameters.put(Names.initialText, initialText );
        }
    }
    public Vector<TTSChunk> getInitialPrompt() {
        if (parameters.get(Names.initialPrompt) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)parameters.get(Names.initialPrompt);
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
    public void setInitialPrompt( Vector<TTSChunk> initialPrompt ) {
        if (initialPrompt != null) {
            parameters.put(Names.initialPrompt, initialPrompt );
        }
    }
    public InteractionMode getInteractionMode() {
        Object obj = parameters.get(Names.interactionMode);
        if (obj instanceof InteractionMode) {
            return (InteractionMode) obj;
        } else if (obj instanceof String) {
            InteractionMode theCode = null;
            try {
                theCode = InteractionMode.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.interactionMode, e);
            }
            return theCode;
        }
        return null;
    }
    public void setInteractionMode( InteractionMode interactionMode ) {
        if (interactionMode != null) {
            parameters.put(Names.interactionMode, interactionMode );
        }
    }
    public Vector<Integer> getInteractionChoiceSetIDList() {
    	if(parameters.get(Names.interactionChoiceSetIDList) instanceof Vector<?>){
    		Vector<?> list = (Vector<?>)parameters.get(Names.interactionChoiceSetIDList);
    		if(list != null && list.size()>0){
        		Object obj = list.get(0);
        		if(obj instanceof Integer){
        			return (Vector<Integer>) list;
        		}
    		}
    	}
        return null;
    }
    public void setInteractionChoiceSetIDList( Vector<Integer> interactionChoiceSetIDList ) {
        if (interactionChoiceSetIDList != null) {
            parameters.put(Names.interactionChoiceSetIDList, interactionChoiceSetIDList );
        }
    }
    public Vector<TTSChunk> getHelpPrompt() {
        if(parameters.get(Names.helpPrompt) instanceof Vector<?>){
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
    public Integer getTimeout() {
        return (Integer) parameters.get( Names.timeout );
    }
    public void setTimeout( Integer timeout ) {
        if (timeout != null) {
            parameters.put(Names.timeout, timeout );
        }
    }
}
