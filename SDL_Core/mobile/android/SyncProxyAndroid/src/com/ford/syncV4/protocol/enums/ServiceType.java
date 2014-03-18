package com.ford.syncV4.protocol.enums;

import com.ford.syncV4.util.ByteEnumer;

import java.util.Vector;


public class ServiceType extends ByteEnumer {

	private static Vector theList = new Vector();
	public static Vector getList() { return theList; }
	
	byte i = 0x00;
	
	protected ServiceType(byte value, String name) {super(value, name);}
    public final static ServiceType Heartbeat = new ServiceType((byte) 0, "Heartbeat_Service");
    public final static ServiceType RPC = new ServiceType((byte)0x07, "RPC");
	public final static ServiceType Bulk_Data = new ServiceType((byte)0xF, "Bulk_Data");
    public final static ServiceType Mobile_Nav = new ServiceType((byte)0xB, "Mobile_Nav");
    public final static ServiceType Audio_Service = new ServiceType((byte)10, "Audio_Service");

	static {
		theList.addElement(RPC);
		theList.addElement(Bulk_Data);
        theList.addElement(Mobile_Nav);
        theList.addElement(Audio_Service);
        theList.addElement(Heartbeat);
	}
	
	public static ServiceType valueOf(byte passedButton) {
		return (ServiceType) get(theList, passedButton);
	}
	
	public static ServiceType[] values() {
		return (ServiceType[]) theList.toArray();
	}
}
