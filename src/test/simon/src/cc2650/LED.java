package cc2650;

public class LED {
	private static final int RED_PIN = 6;
	private static final int GREEN_PIN = 7;

	private static GpioPin sRed = null;
	private static GpioPin sGreen = null;
	
	public static synchronized GpioPin Red() {
		if (sRed == null) {
			sRed = new GpioPin(RED_PIN);
		}
		
		return sRed;
	}
	
	public static synchronized GpioPin Green() {
		if (sGreen == null) {
			sGreen = new GpioPin(GREEN_PIN);
		}
		
		return sGreen;
	}
}