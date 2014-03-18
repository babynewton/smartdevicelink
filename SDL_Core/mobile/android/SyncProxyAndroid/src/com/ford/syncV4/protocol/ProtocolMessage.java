package com.ford.syncV4.protocol;

import com.ford.syncV4.protocol.enums.MessageType;
import com.ford.syncV4.protocol.enums.ServiceType;

public class ProtocolMessage {
	private byte version = 1;
	private ServiceType _serviceType = ServiceType.RPC;
	private MessageType _messageType = MessageType.UNDEFINED;
	private byte _sessionID = 0;
	private byte _rpcType;
	private int _functionID;
	private int _correlationID;
	private int _jsonSize;
	
	private byte[] _data = null;
	private byte[] _bulkData = null;

    public static byte RPCTYPE_REQUEST = 0x00;
    public static byte RPCTYPE_RESPONSE = 0x01;
    public static byte RPCTYPE_NOTIFICATION = 0x02;

	public ProtocolMessage() {}

    /**
     * Copy constructor. Makes a deep copy of the given ProtocolMessage object.
     *
     * @param that object to copy
     */
    public ProtocolMessage(ProtocolMessage that) {
        this.version = that.version;
        this._serviceType = that._serviceType;
        this._messageType = that._messageType;
        this._sessionID = that._sessionID;
        this._rpcType = that._rpcType;
        this._functionID = that._functionID;
        this._correlationID = that._correlationID;
        this._jsonSize = that._jsonSize;
        if (null != that._data) {
            this._data = that._data.clone();
        }
        if (null != that._bulkData) {
            this._bulkData = that._bulkData.clone();
        }
    }

	public byte getVersion() {
		return version;
	}

	public void setVersion(byte version) {
		this.version = version;
	}

	public byte getSessionID() {
		return _sessionID;
	}

	public void setSessionID(byte sessionID) {
		this._sessionID = sessionID;
	}

	public byte[] getData() {
		return _data;
	}

    public void setData(byte[] data) {
        this._data = new byte[data.length];
        System.arraycopy(data, 0, this._data, 0, _data.length);
        this._jsonSize = data.length;
    }

    public void setData(byte[] data, int length) {
        if (this._data != null)
            this._data = null;
        this._data = new byte[length];
        System.arraycopy(data, 0, this._data, 0, length);
        this._jsonSize = 0;
    }

	public byte[] getBulkData() {
		return _bulkData;
	}

	public void setBulkData(byte[] bulkData) {
        if (bulkData == null) {
            return;
        }
		if (this._bulkData != null)
			this._bulkData = null;
		this._bulkData = new byte[bulkData.length];
		System.arraycopy(bulkData, 0, this._bulkData, 0, bulkData.length);
		//this._bulkData = bulkData;
	}

	public ServiceType getServiceType() {
		return _serviceType;
	}

	public void setSessionType(ServiceType serviceType) {
		this._serviceType = serviceType;
	}

	public MessageType getMessageType() {
		return _messageType;
	}

	public void setMessageType(MessageType messageType) {
		this._messageType = messageType;
	}
	
	public byte getRPCType() {
		return _rpcType;
	}
	
	public void setRPCType(byte _rpcType) {
		this._rpcType = _rpcType;
	}
	
	public int getFunctionID() {
		return _functionID;
	}
	
	public void setFunctionID(int _functionID) {
		this._functionID = _functionID;
	}
	
	public int getCorrID() {
		return _correlationID;
	}
	
	public void setCorrID(int _correlationID) {
		this._correlationID = _correlationID;
	}

	public int getJsonSize() {
		return _jsonSize;
	}

	public void setJsonSize(int _jsonSize) {
		this._jsonSize = _jsonSize;
	}
} // end-class