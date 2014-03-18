package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.HmiZoneCapabilities;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.proxy.rpc.enums.PrerecordedSpeech;
import com.ford.syncV4.proxy.rpc.enums.SpeechCapabilities;
import com.ford.syncV4.proxy.rpc.enums.VrCapabilities;
import com.ford.syncV4.util.DebugTool;

public class RegisterAppInterfaceResponse extends RPCResponse {

    public RegisterAppInterfaceResponse() {
        super("RegisterAppInterface");
    }
    public RegisterAppInterfaceResponse(Hashtable hash) {
        super(hash);
    }
    public SyncMsgVersion getSyncMsgVersion() {
        Object obj = parameters.get(Names.syncMsgVersion);
        if (obj instanceof SyncMsgVersion) {
        	return (SyncMsgVersion)obj;
        } else if (obj instanceof Hashtable) {
        	return new SyncMsgVersion((Hashtable)obj);
        }
        return null;
    }
    public void setSyncMsgVersion(SyncMsgVersion syncMsgVersion) {
        if (syncMsgVersion != null) {
            parameters.put(Names.syncMsgVersion, syncMsgVersion);
        }
    }
    public Language getLanguage() {
        Object obj = parameters.get(Names.language);
        if (obj instanceof Language) {
            return (Language) obj;
        } else if (obj instanceof String) {
            Language theCode = null;
            try {
                theCode = Language.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.language, e);
            }
            return theCode;
        }
        return null;
    }
    public void setLanguage(Language language) {
        if (language != null) {
            parameters.put(Names.language, language);
        }
    }
    public Language getHmiDisplayLanguage() {
        Object obj = parameters.get(Names.hmiDisplayLanguage);
        if (obj instanceof Language) {
            return (Language) obj;
        } else if (obj instanceof String) {
            Language theCode = null;
            try {
                theCode = Language.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.hmiDisplayLanguage, e);
            }
            return theCode;
        }
        return null;
    }
    public void setHmiDisplayLanguage(Language hmiDisplayLanguage) {
        if (hmiDisplayLanguage != null) {
            parameters.put(Names.hmiDisplayLanguage, hmiDisplayLanguage);
        } else {
        	parameters.remove(Names.hmiDisplayLanguage);
        }
    }
    public DisplayCapabilities getDisplayCapabilities() {
        Object obj = parameters.get(Names.displayCapabilities);
        if (obj instanceof DisplayCapabilities) {
        	return (DisplayCapabilities)obj;
        } else if (obj instanceof Hashtable) {
        	return new DisplayCapabilities((Hashtable)obj);
        }
        return null;
    }
    public void setDisplayCapabilities(DisplayCapabilities displayCapabilities) {
        if (displayCapabilities != null) {
            parameters.put(Names.displayCapabilities, displayCapabilities);
        }
    }
    public Vector<ButtonCapabilities> getButtonCapabilities() {
        if (parameters.get(Names.buttonCapabilities) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)parameters.get(Names.buttonCapabilities);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof ButtonCapabilities) {
	                return (Vector<ButtonCapabilities>) list;
	            } else if (obj instanceof Hashtable) {
	                Vector<ButtonCapabilities> newList = new Vector<ButtonCapabilities>();
	                for (Object hashObj : list) {
	                    newList.add(new ButtonCapabilities((Hashtable)hashObj));
	                }
	                return newList;
	            }
	        }
        }
        return null;
    }
    public void setButtonCapabilities(Vector<ButtonCapabilities> buttonCapabilities) {
        if (buttonCapabilities != null) {
            parameters.put(Names.buttonCapabilities, buttonCapabilities);
        }
    }
    public Vector<SoftButtonCapabilities> getSoftButtonCapabilities() {
        if (parameters.get(Names.softButtonCapabilities) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)parameters.get(Names.softButtonCapabilities);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof SoftButtonCapabilities) {
	                return (Vector<SoftButtonCapabilities>) list;
	            } else if (obj instanceof Hashtable) {
	                Vector<SoftButtonCapabilities> newList = new Vector<SoftButtonCapabilities>();
	                for (Object hashObj : list) {
	                    newList.add(new SoftButtonCapabilities((Hashtable)hashObj));
	                }
	                return newList;
	            }
	        }
        }
        return null;
    }
    public void setSoftButtonCapabilities(Vector<SoftButtonCapabilities> softButtonCapabilities) {
        if (softButtonCapabilities != null) {
            parameters.put(Names.softButtonCapabilities, softButtonCapabilities);
        }
    }
    public PresetBankCapabilities getPresetBankCapabilities() {
        Object obj = parameters.get(Names.presetBankCapabilities);
        if (obj instanceof PresetBankCapabilities) {
        	return (PresetBankCapabilities)obj;
        } else if (obj instanceof Hashtable) {
        	return new PresetBankCapabilities((Hashtable)obj);
        }
        return null;
    }
    public void setPresetBankCapabilities(PresetBankCapabilities presetBankCapabilities) {
        if (presetBankCapabilities != null) {
            parameters.put(Names.presetBankCapabilities, presetBankCapabilities);
        }
    }
    public Vector<HmiZoneCapabilities> getHmiZoneCapabilities() {
        if (parameters.get(Names.hmiZoneCapabilities) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)parameters.get(Names.hmiZoneCapabilities);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof HmiZoneCapabilities) {
	                return (Vector<HmiZoneCapabilities>) list;
	            } else if (obj instanceof String) {
	                Vector<HmiZoneCapabilities> newList = new Vector<HmiZoneCapabilities>();
	                for (Object hashObj : list) {
	                    String strFormat = (String)hashObj;
	                    HmiZoneCapabilities toAdd = null;
	                    try {
	                        toAdd = HmiZoneCapabilities.valueForString(strFormat);
	                    } catch (Exception e) {
	                    	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.hmiZoneCapabilities, e);
	                    }
	                    if (toAdd != null) {
	                        newList.add(toAdd);
	                    }
	                }
	                return newList;
	            }
	        }
        }
        return null;
    }
    public void setHmiZoneCapabilities(Vector<HmiZoneCapabilities> hmiZoneCapabilities) {
        if (hmiZoneCapabilities != null) {
            parameters.put(Names.hmiZoneCapabilities, hmiZoneCapabilities);
        }
    }
    public Vector<SpeechCapabilities> getSpeechCapabilities() {
        if (parameters.get(Names.speechCapabilities) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)parameters.get(Names.speechCapabilities);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof SpeechCapabilities) {
	                return (Vector<SpeechCapabilities>) list;
	            } else if (obj instanceof String) {
	                Vector<SpeechCapabilities> newList = new Vector<SpeechCapabilities>();
	                for (Object hashObj : list) {
	                    String strFormat = (String)hashObj;
	                    SpeechCapabilities toAdd = null;
	                    try {
	                        toAdd = SpeechCapabilities.valueForString(strFormat);
	                    } catch (Exception e) {
	                    	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.speechCapabilities, e);
	                    }
	                    if (toAdd != null) {
	                        newList.add(toAdd);
	                    }
	                }
	                return newList;
	            }
	        }
        }
        return null;
    }
    public void setSpeechCapabilities(Vector<SpeechCapabilities> speechCapabilities) {
        if (speechCapabilities != null) {
            parameters.put(Names.speechCapabilities, speechCapabilities);
        }
    }

    public Vector<PrerecordedSpeech> getPrerecordedSpeech() {
        final Object o = parameters.get(Names.prerecordedSpeech);
        if (o instanceof Vector<?>) {
            Vector<?> list = (Vector<?>) o;
            if (list != null && list.size() > 0) {
                Object obj = list.get(0);
                if (obj instanceof PrerecordedSpeech) {
                    return (Vector<PrerecordedSpeech>) list;
                } else if (obj instanceof String) {
                    Vector<PrerecordedSpeech> newList =
                            new Vector<PrerecordedSpeech>();
                    for (Object hashObj : list) {
                        String strFormat = (String) hashObj;
                        PrerecordedSpeech toAdd = null;
                        try {
                            toAdd = PrerecordedSpeech.valueForString(strFormat);
                        } catch (Exception e) {
                            DebugTool.logError("Failed to parse " +
                                    getClass().getSimpleName() + "." +
                                    Names.prerecordedSpeech, e);
                        }
                        if (toAdd != null) {
                            newList.add(toAdd);
                        }
                    }
                    return newList;
                }
            }
        }
        return null;
    }

    public void setPrerecordedSpeech(
            Vector<PrerecordedSpeech> prerecordedSpeech) {
        if (prerecordedSpeech != null) {
            parameters.put(Names.prerecordedSpeech, prerecordedSpeech);
        } else {
            parameters.remove(Names.prerecordedSpeech);
        }
    }

    public Vector<VrCapabilities> getVrCapabilities() {
        if (parameters.get(Names.vrCapabilities) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)parameters.get(Names.vrCapabilities);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof VrCapabilities) {
	                return (Vector<VrCapabilities>) list;
	            } else if (obj instanceof String) {
	                Vector<VrCapabilities> newList = new Vector<VrCapabilities>();
	                for (Object hashObj : list) {
	                    String strFormat = (String)hashObj;
	                    VrCapabilities toAdd = null;
	                    try {
	                        toAdd = VrCapabilities.valueForString(strFormat);
	                    } catch (Exception e) {
	                    	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.vrCapabilities, e);
	                    }
	                    if (toAdd != null) {
	                        newList.add(toAdd);
	                    }
	                }
	                return newList;
	            }
	        }
        }
        return null;
    }
    public void setVrCapabilities(Vector<VrCapabilities> vrCapabilities) {
        if (vrCapabilities != null) {
            parameters.put(Names.vrCapabilities, vrCapabilities);
        }
    }

    public Vector<AudioPassThruCapabilities> getAudioPassThruCapabilities() {
        final Object o = parameters.get(Names.audioPassThruCapabilities);
        if (o instanceof Vector<?>) {
            Vector<?> list = (Vector<?>) o;
            if (list != null && list.size() > 0) {
                Object obj = list.get(0);
                if (obj instanceof AudioPassThruCapabilities) {
                    return (Vector<AudioPassThruCapabilities>) list;
                } else if (obj instanceof String) {
                    Vector<AudioPassThruCapabilities> newList =
                            new Vector<AudioPassThruCapabilities>();
                    for (Object hashObj : list) {
                        AudioPassThruCapabilities toAdd = null;
                        try {
                            toAdd = new AudioPassThruCapabilities(
                                    (Hashtable) hashObj);
                        } catch (Exception e) {
                            DebugTool.logError("Failed to parse " +
                                    getClass().getSimpleName() + "." +
                                    Names.audioPassThruCapabilities, e);
                        }
                        if (toAdd != null) {
                            newList.add(toAdd);
                        }
                    }
                    return newList;
                }
            }
        }
        return null;
    }

    public void setAudioPassThruCapabilities(
            Vector<AudioPassThruCapabilities> audioPassThruCapabilities) {
        if (audioPassThruCapabilities != null) {
            parameters.put(Names.audioPassThruCapabilities,
                    audioPassThruCapabilities);
        } else {
            parameters.remove(Names.audioPassThruCapabilities);
        }
    }

    public VehicleType getVehicleType() {
        Object obj = parameters.get(Names.vehicleType);
        if (obj instanceof VehicleType) {
        	return (VehicleType)obj;
        } else if (obj instanceof Hashtable) {
        	return new VehicleType((Hashtable)obj);
        }
        return null;
    }
    public void setVehicleType(VehicleType vehicleType) {
        if (vehicleType != null) {
            parameters.put(Names.vehicleType, vehicleType);
        }
    }

    public Vector<Integer> getSupportedDiagModes() {
        final Object o = parameters.get(Names.supportedDiagModes);
        if (o instanceof Vector<?>) {
            Vector<?> list = (Vector<?>) o;
            if (list != null && list.size() > 0) {
                Object obj = list.get(0);
                if (obj instanceof Integer) {
                    return (Vector<Integer>) list;
                }
            }
        }
        return null;
    }

    public void setSupportedDiagModes(Vector<Integer> supportedDiagModes) {
        if (supportedDiagModes != null) {
            parameters.put(Names.supportedDiagModes, supportedDiagModes);
        } else {
            parameters.remove(Names.supportedDiagModes);
        }
    }
}
