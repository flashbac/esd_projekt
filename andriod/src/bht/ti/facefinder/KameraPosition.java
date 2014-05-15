package bht.ti.facefinder;

public enum KameraPosition {
	Left("left"),
	Right("right"),
	Top("top"),
	Bottom("bottom")
	;
	
	private KameraPosition(final String text) {
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
