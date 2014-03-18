package com.ford.syncV4.android.activity.mobilenav;

import android.os.AsyncTask;
import android.util.Log;

import com.ford.syncV4.android.activity.SyncProxyTester;

import java.io.IOException;
import java.io.OutputStream;

public class FileStreamingLogic {

    private static final String TAG = "FileStreamingLogic";

    private StaticFileReader staticFileReader;
    private OutputStream outputStream;
    private ServicePreviewFragmentInterface context;
    private Integer fileResID;
    /**
     * Indicates whether stream is completed or not.
     */
    private boolean mIsStreamingInProgress;

    public FileStreamingLogic(ServicePreviewFragmentInterface mobileNavPreviewFragment) {
        context = mobileNavPreviewFragment;
    }

    public OutputStream getOutputStream() {
        return outputStream;
    }

    public void setOutputStream(OutputStream stream) {
        this.outputStream = stream;
    }

    public Integer getFileResID() {
        return fileResID;
    }

    public void setFileResID(Integer fileResID) {
        this.fileResID = fileResID;
    }

    public void cancelStreaming() {
        if (staticFileReader != null) {
            staticFileReader.cancel(true);
        }
    }

    public void resetStreaming() {
        mIsStreamingInProgress = false;
        cancelStreaming();
    }

    public void startFileStreaming() {
        if (staticFileReader == null || staticFileReader.getStatus() == AsyncTask.Status.FINISHED){
            createStaticFileReader();
        }
        staticFileReader.execute(fileResID);
    }

    public boolean isStreamingInProgress() {
        return mIsStreamingInProgress;
    }

    public void createStaticFileReader() {

        staticFileReader = new StaticFileReader(context.getActivity(), new DataReaderListener() {

            @Override
            public void onStartReading() {
                Log.d(TAG, "On Start reading");
                mIsStreamingInProgress = true;
                context.dataStreamingStarted();
            }

            @Override
            public void onDataReceived(final byte[] data) {
                if (outputStream != null && data != null) {
                    try {
                        //Log.d(TAG, "On read data:" + data);
                        outputStream.write(data);
                    } catch (IOException e) {
                        Log.e(TAG, "FIle streamer error", e);
                       cancelStreaming();
                       SyncProxyTester tester = (SyncProxyTester) context.getActivity();

                       tester.logError(e);
                    }
                }
            }

            @Override
            public void onCancelReading() {
                Log.d(TAG, "On Cancel reading");
                context.dataStreamingStopped();
            }

            @Override
            public void onEndReading() {
                Log.d(TAG, "On Complete reading");
                mIsStreamingInProgress = false;
                context.dataStreamingStopped();
            }
        });
    }
}