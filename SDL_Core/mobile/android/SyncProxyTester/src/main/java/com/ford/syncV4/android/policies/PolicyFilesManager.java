package com.ford.syncV4.android.policies;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/26/14
 * Time: 9:59 AM
 */

import android.os.Environment;
import android.util.Log;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.activity.SafeToast;
import com.ford.syncV4.android.adapters.LogAdapter;
import com.ford.syncV4.android.manager.AppPreferencesManager;
import com.ford.syncV4.android.utils.AppUtils;
import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.proxy.systemrequest.ISystemRequestProxy;

import java.io.File;

/**
 * This class provide possibility to save Policy Table Snapshot file (which comes from SDL)
 * and to send back Policy Table Update file
 */
public class PolicyFilesManager {

    /**
     * Save Policy Table Snapshot data to file
     *
     * @param data Policy Table Snapshot data
     */
    public static void savePolicyTableSnapshot(byte[] data) {
        //final byte[] fileData = AppUtils.contentsOfResource(R.raw.policy_table_shanpshot);
        String mTMPFilePath = Environment.getExternalStorageDirectory() +
                "/policyTableSnapshot.json";

        boolean result = AppUtils.saveDataToFile(data, mTMPFilePath);
        if (result) {
            SafeToast.showToastAnyThread("File '" + mTMPFilePath + "' successfully saved");
        } else {
            SafeToast.showToastAnyThread("File '" + mTMPFilePath + "' could not be save");
        }
    }

    /**
     * Send Policy Table Update data to SDL
     *
     * @param proxy {@link com.ford.syncV4.proxy.systemrequest.ISystemRequestProxy} implementation
     * @param logAdapter Log Adapter (TO BE REMOVED)
     */
    public static void sendPolicyTableUpdate(ISystemRequestProxy proxy, LogAdapter logAdapter) {
        String mPolicyTableUpdatePath = AppPreferencesManager.getPolicyTableUpdateFilePath();
        //Environment.getExternalStorageDirectory() +
        //        "/policyTableUpdate.json";

        byte[] data = null;
        if (mPolicyTableUpdatePath.equals("")) {
            data = AppUtils.contentsOfResource(R.raw.policy_table_update);
        } else {
            File mPolicyUpdateFile = new File(mPolicyTableUpdatePath);
            if (mPolicyUpdateFile.exists()) {
                data = AppUtils.contentsOfResource(mPolicyUpdateFile);
            }
        }

        if (data == null) {
            SafeToast.showToastAnyThread("Policy Snapshot could not be found");
            // TODO : Logging to be refactored
            if (logAdapter != null) {
                logAdapter.logMessage("Policy Snapshot could not be found", Log.ERROR, true);
            }
            return;
        }

        SafeToast.showToastAnyThread("Policy Update is found");

        // TODO : Logging to be refactored
        if (logAdapter != null) {
            logAdapter.logMessage("Policy Update is found", Log.DEBUG, true);
        }

        try {
            String mPolicyTableUpdateFileName = "PolicyTableUpdate";
            proxy.putPolicyTableUpdateFile(mPolicyTableUpdateFileName, data);
            SafeToast.showToastAnyThread("Policy Update sent");
            if (logAdapter != null) {
                logAdapter.logMessage("Policy Update sent", Log.DEBUG, true);
            }
        } catch (SyncException e) {
            // TODO : Logging to be refactored
            if (logAdapter != null) {
                logAdapter.logMessage("Can't upload policy table update mPolicyUpdateFile:" +
                        e.getMessage(), Log.ERROR, true);
            }
        }
    }
}