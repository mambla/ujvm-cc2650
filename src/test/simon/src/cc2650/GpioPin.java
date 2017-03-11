package cc2650;

public class GpioPin {
	private int mGpioPin = 0;

	public GpioPin(int pin) {
		mGpioPin = pin;
	}
	
	public void set(boolean val) {
		UC.gpioSetHi(0, mGpioPin, val);
	}
	
	public boolean get() {
		return UC.gpioIsHi(0, mGpioPin);
	}
	
	public void toggle() {
		boolean current = get();
		set(!current);
	}
	
	public void flash(int shineMs) {
		set(true);
		Thread.sleep(shineMs);
		set(false);
	}
}
