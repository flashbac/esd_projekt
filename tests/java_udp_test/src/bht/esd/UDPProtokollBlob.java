package bht.esd;

public class UDPProtokollBlob {
	int id;
	byte[] data;
	
	public UDPProtokollBlob(int id, byte[] data) {
		// TODO Auto-generated constructor stub
		this.id = id;
		this.data = data;
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public byte[] getData() {
		return data;
	}

	public void setData(byte[] data) {
		this.data = data;
	}

}
