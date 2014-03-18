package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCNotification;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.ComponentVolumeStatus;
import com.ford.syncV4.proxy.rpc.enums.PRNDL;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataEventStatus;
import com.ford.syncV4.proxy.rpc.enums.WiperStatus;
import com.ford.syncV4.util.DebugTool;

public class OnVehicleData extends RPCNotification {

    public OnVehicleData() {
        super("OnVehicleData");
    }
    public OnVehicleData(Hashtable hash) {
        super(hash);
    }
    public void setGps(GPSData gps) {
    	if (gps != null) {
    		parameters.put(Names.gps, gps);
    	} else {
    		parameters.remove(Names.gps);
    	}
    }
    public GPSData getGps() {
    	Object obj = parameters.get(Names.gps);
        if (obj instanceof GPSData) {
            return (GPSData) obj;
        } else {
        	GPSData theCode = null;
            try {
                theCode = new GPSData((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.gps, e);
            }
            return theCode;
        }
    }
    public void setSpeed(Integer speed) {
    	if (speed != null) {
    		store.put(Names.speed, speed);
    	} else {
    		store.remove(Names.speed);
    	}
    }
    public Integer getSpeed() {
    	return (Integer) store.get(Names.speed);
    }
    public void setRpm(Integer rpm) {
    	if (rpm != null) {
    		store.put(Names.rpm, rpm);
    	} else {
    		store.remove(Names.rpm);
    	}
    }
    public Integer getRpm() {
    	return (Integer) store.get(Names.rpm);
    }
    public void setFuelLevel(Float fuelLevel) {
    	if (fuelLevel != null) {
    		store.put(Names.fuelLevel, fuelLevel);
    	} else {
    		store.remove(Names.fuelLevel);
    	}
    }
    public Float getFuelLevel() {
    	return (Float) store.get(Names.fuelLevel);
    }
    public void setFuelLevel_State(ComponentVolumeStatus fuelLevel_State) {
        if (fuelLevel_State != null) {
            parameters.put(Names.fuelLevel_State, fuelLevel_State);
        } else {
            parameters.remove(Names.fuelLevel_State);
        }
    }
    public ComponentVolumeStatus getFuelLevel_State() {
        Object obj = parameters.get(Names.fuelLevel_State);
        if (obj instanceof ComponentVolumeStatus) {
            return (ComponentVolumeStatus) obj;
        } else if (obj instanceof String) {
            ComponentVolumeStatus theCode = null;
            try {
                theCode = ComponentVolumeStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.fuelLevel_State, e);
            }
            return theCode;
        }
        return null;
    }
    public void setInstantFuelConsumption(Float instantFuelConsumption) {
        if (instantFuelConsumption != null) {
            parameters.put(Names.instantFuelConsumption, instantFuelConsumption);
        } else {
            parameters.remove(Names.instantFuelConsumption);
        }
    }
    public Float getInstantFuelConsumption() {
        return (Float) parameters.get(Names.instantFuelConsumption);
    }
    public void setExternalTemperature(Float externalTemperature) {
    	if (externalTemperature != null) {
    		store.put(Names.externalTemperature, externalTemperature);
    	} else {
    		store.remove(Names.externalTemperature);
    	}
    }
    public Float getExternalTemperature() {
    	return (Float) store.get(Names.externalTemperature);
    }
    public void setVin(String vin) {
    	if (vin != null) {
    		store.put(Names.vin, vin);
    	} else {
    		store.remove(Names.vin);
    	}
    }
    public String getVin() {
    	return (String) store.get(Names.vin);
    }
    public void setPrndl(PRNDL prndl) {
    	if (prndl != null) {
    		store.put(Names.prndl, prndl);
    	} else {
    		store.remove(Names.prndl);
    	}
    }
    public PRNDL getPrndl() {
        Object obj = store.get(Names.prndl);
        if (obj instanceof PRNDL) {
            return (PRNDL) obj;
        } else if (obj instanceof String) {
        	PRNDL theCode = null;
            try {
                theCode = PRNDL.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.prndl, e);
            }
            return theCode;
        }
        return null;
    }
    public void setTirePressure(TireStatus tirePressure) {
    	if (tirePressure != null) {
    		store.put(Names.tirePressure, tirePressure);
    	} else {
    		store.remove(Names.tirePressure);
    	}
    }
    public TireStatus getTirePressure() {
    	Object obj = store.get(Names.tirePressure);
        if (obj instanceof TireStatus) {
            return (TireStatus) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new TireStatus((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.tirePressure, e);
            }
        }
        return null;
    }
    public void setOdometer(Integer odometer) {
        if (odometer != null) {
            store.put(Names.odometer, odometer);
        } else {
            store.remove(Names.odometer);
        }
    }
    public Integer getOdometer() {
        return (Integer) store.get(Names.odometer);
    }
    public void setBeltStatus(BeltStatus beltStatus) {
        if (beltStatus != null) {
            parameters.put(Names.beltStatus, beltStatus);
        } else {
            parameters.remove(Names.beltStatus);
        }
    }
    public BeltStatus getBeltStatus() {
        Object obj = parameters.get(Names.beltStatus);
        if (obj instanceof BeltStatus) {
            return (BeltStatus) obj;
        } else if (obj instanceof Hashtable) {
            try {
                return new BeltStatus((Hashtable) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.beltStatus, e);
            }
        }
        return null;
    }
    public void setBodyInformation(BodyInformation bodyInformation) {
        if (bodyInformation != null) {
            parameters.put(Names.bodyInformation, bodyInformation);
        } else {
            parameters.remove(Names.bodyInformation);
        }
    }
    public BodyInformation getBodyInformation() {
        Object obj = parameters.get(Names.bodyInformation);
        if (obj instanceof BodyInformation) {
            return (BodyInformation) obj;
        } else if (obj instanceof Hashtable) {
            try {
                return new BodyInformation((Hashtable) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.bodyInformation, e);
            }
        }
        return null;
    }
    public void setDeviceStatus(DeviceStatus deviceStatus) {
        if (deviceStatus != null) {
            parameters.put(Names.deviceStatus, deviceStatus);
        } else {
            parameters.remove(Names.deviceStatus);
        }
    }
    public DeviceStatus getDeviceStatus() {
        Object obj = parameters.get(Names.deviceStatus);
        if (obj instanceof DeviceStatus) {
            return (DeviceStatus) obj;
        } else if (obj instanceof Hashtable) {
            try {
                return new DeviceStatus((Hashtable) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.deviceStatus, e);
            }
        }
        return null;
    }
    public void setDriverBraking(VehicleDataEventStatus driverBraking) {
        if (driverBraking != null) {
            parameters.put(Names.driverBraking, driverBraking);
        } else {
            parameters.remove(Names.driverBraking);
        }
    }
    public VehicleDataEventStatus getDriverBraking() {
        Object obj = parameters.get(Names.driverBraking);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
            VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.driverBraking, e);
            }
            return theCode;
        }
        return null;
    }
    public void setWiperStatus(WiperStatus wiperStatus) {
        if (wiperStatus != null) {
            parameters.put(Names.wiperStatus, wiperStatus);
        } else {
            parameters.remove(Names.wiperStatus);
        }
    }
    public WiperStatus getWiperStatus() {
        Object obj = parameters.get(Names.wiperStatus);
        if (obj instanceof WiperStatus) {
            return (WiperStatus) obj;
        } else if (obj instanceof String) {
            WiperStatus theCode = null;
            try {
                theCode = WiperStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.wiperStatus, e);
            }
            return theCode;
        }
        return null;
    }

    public void setHeadLampStatus(HeadLampStatus headLampStatus) {
        if (headLampStatus != null) {
            parameters.put(Names.headLampStatus, headLampStatus);
        } else {
            parameters.remove(Names.headLampStatus);
        }
    }

    public HeadLampStatus getHeadLampStatus() {
        Object obj = parameters.get(Names.headLampStatus);
        if (obj instanceof HeadLampStatus) {
            return (HeadLampStatus) obj;
        } else if (obj instanceof Hashtable) {
            try {
                return new HeadLampStatus((Hashtable) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.headLampStatus, e);
            }
        }
        return null;
    }

    public void setEngineTorque(Integer engineTorque) {
    	if (engineTorque != null) {
    		store.put(Names.engineTorque, engineTorque);
    	} else {
    		store.remove(Names.engineTorque);
    	}
    }
    public Integer getEngineTorque() {
    	return (Integer) store.get(Names.engineTorque);
    }
    public void setAccPedalPosition(Float accPedalPosition) {
        if (accPedalPosition != null) {
            parameters.put(Names.accPedalPosition, accPedalPosition);
        } else {
            parameters.remove(Names.accPedalPosition);
        }
    }
    public Float getAccPedalPosition() {
        return (Float) parameters.get(Names.accPedalPosition);
    }

    public void setSteeringWheelAngle(Float steeringWheelAngle) {
        if (steeringWheelAngle != null) {
            parameters.put(Names.steeringWheelAngle, steeringWheelAngle);
        } else {
            parameters.remove(Names.steeringWheelAngle);
        }
    }

    public Float getSteeringWheelAngle() {
        final Object o = parameters.get(Names.steeringWheelAngle);
        if (o instanceof Float) {
            return (Float) o;
        }
        if (o instanceof Double) {
            return ((Double) o).floatValue();
        }
        return null;
    }

    public void setECallInfo(ECallInfo eCallInfo) {
        if (eCallInfo != null) {
            parameters.put(Names.eCallInfo, eCallInfo);
        } else {
            parameters.remove(Names.eCallInfo);
        }
    }

    public ECallInfo getECallInfo() {
        Object obj = parameters.get(Names.eCallInfo);
        if (obj instanceof ECallInfo) {
            return (ECallInfo) obj;
        } else if (obj instanceof Hashtable) {
            try {
                return new ECallInfo((Hashtable) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.eCallInfo, e);
            }
        }
        return null;
    }

    public void setAirbagStatus(AirbagStatus airbagStatus) {
        if (airbagStatus != null) {
            parameters.put(Names.airbagStatus, airbagStatus);
        } else {
            parameters.remove(Names.airbagStatus);
        }
    }

    public AirbagStatus getAirbagStatus() {
        Object obj = parameters.get(Names.airbagStatus);
        if (obj instanceof AirbagStatus) {
            return (AirbagStatus) obj;
        } else if (obj instanceof Hashtable) {
            try {
                return new AirbagStatus((Hashtable) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.airbagStatus, e);
            }
        }
        return null;
    }

    public void setEmergencyEvent(EmergencyEvent emergencyEvent) {
        if (emergencyEvent != null) {
            parameters.put(Names.emergencyEvent, emergencyEvent);
        } else {
            parameters.remove(Names.emergencyEvent);
        }
    }

    public EmergencyEvent getEmergencyEvent() {
        Object obj = parameters.get(Names.emergencyEvent);
        if (obj instanceof EmergencyEvent) {
            return (EmergencyEvent) obj;
        } else if (obj instanceof Hashtable) {
            try {
                return new EmergencyEvent((Hashtable) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.emergencyEvent, e);
            }
        }
        return null;
    }

    public void setClusterModeStatus(ClusterModeStatus clusterModeStatus) {
        if (clusterModeStatus != null) {
            parameters.put(Names.clusterModeStatus, clusterModeStatus);
        } else {
            parameters.remove(Names.clusterModeStatus);
        }
    }

    public ClusterModeStatus getClusterModeStatus() {
        Object obj = parameters.get(Names.clusterModeStatus);
        if (obj instanceof ClusterModeStatus) {
            return (ClusterModeStatus) obj;
        } else if (obj instanceof Hashtable) {
            try {
                return new ClusterModeStatus((Hashtable) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.clusterModeStatus, e);
            }
        }
        return null;
    }

    public void setMyKey(MyKey myKey) {
        if (myKey != null) {
            parameters.put(Names.myKey, myKey);
        } else {
            parameters.remove(Names.myKey);
        }
    }

    public MyKey getMyKey() {
        Object obj = parameters.get(Names.myKey);
        if (obj instanceof MyKey) {
            return (MyKey) obj;
        } else if (obj instanceof Hashtable) {
            try {
                return new MyKey((Hashtable) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.myKey, e);
            }
        }
        return null;
    }
}