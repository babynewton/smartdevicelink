package com.ford.syncV4.android.activity.mobilenav;

import android.content.Context;
import android.widget.CheckBox;

import com.ford.syncV4.android.R;

/**
 * Created by Andrew Batutin on 1/24/14.
 */
public class AudioServiceCheckboxState extends CheckBoxState {

    public AudioServiceCheckboxState(CheckBox item, Context context) {
        super(item, context);
        hintString = getResources().getString(R.string.audio_service_hint);
        textString = getResources().getString(R.string.audio_service_on);
        setStateOff();
    }
}
