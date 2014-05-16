package bht.ti.facefinder;

public enum KameraMode {
	Auto("auto"),
	Manual("manu")
	;
	
	private KameraMode(final String text) {
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

