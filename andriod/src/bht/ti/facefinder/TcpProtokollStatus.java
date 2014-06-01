package bht.ti.facefinder;

public enum TcpProtokollStatus {
	cams("cams"),
	camera("camera"),
	udp("udp"),
	face("face"),
	track("track"),
	position("position")
	;
	
	private TcpProtokollStatus(final String text) {
        this.text = text;
    }
	
	private final String text;
	
    /* (non-Javadoc)
     * @see java.lang.Enum#toString()
     */
    @Override
    public String toString() {
        return text;
    }
}
