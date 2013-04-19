//
// Copyright (c) 2013 Ford Motor Company
//
package com.smartdevicelink.proxy.rpc;

import java.util.Hashtable;

import com.smartdevicelink.proxy.RPCStruct;
import com.smartdevicelink.proxy.constants.Names;
import com.smartdevicelink.proxy.rpc.enums.PermissionStatus;
import com.smartdevicelink.util.DebugTool;

public class PermissionGroupItem extends RPCStruct {
	
	public PermissionGroupItem() { }
    public PermissionGroupItem(Hashtable hash) {
        super(hash);
    }
    public String getPermissionGroupName() {
    	Object obj = store.get(Names.PermissionGroupName);
    	if (obj instanceof String) {
    		return (String)obj;
    	}
    	return null;
    }
    public void setPermissionGroupName(String permissionGroupName) {
    	if (permissionGroupName != null) {
    		store.put(Names.PermissionGroupName, permissionGroupName);
    	}
    }
    public PermissionStatus getPermissionGroupStatus() {
    	Object obj = store.get(Names.PermissionGroupStatus);
    	if (obj instanceof PermissionStatus) {
    		return (PermissionStatus)obj;
    	} else if(obj instanceof String) {
    		PermissionStatus theCode = null;
    		try {
    			theCode = PermissionStatus.valueForString((String) obj);
    		} catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.name, e);
            }
    		return theCode;
    	}
    	return null;
    }
    public void setPermissionGroupStatus(PermissionStatus permissionStatus) {
    	if (permissionStatus != null) {
    		store.put(Names.PermissionGroupStatus, permissionStatus);
    	}
    }
}
