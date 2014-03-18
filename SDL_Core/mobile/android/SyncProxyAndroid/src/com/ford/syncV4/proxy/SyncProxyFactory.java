package com.ford.syncV4.proxy;

import android.util.Log;

import com.ford.syncV4.exception.SyncException;

public class SyncProxyFactory {
	
	public static SyncProxy buildSyncProxy(IProxyListener listener) {
		SyncProxy ret = null;
		try {
			ret = new SyncProxy(listener);
		} catch (SyncException e) {
            Log.e("SyncProxyTester", e.toString());
		}
		return ret;
	}
}