package cc2650;
import java.lang.Exception;

public class Button {
	public static final int Both = 0;
	public static final int Button1 = 1;
	public static final int Button2 = 2;	
	
	private int mButton;
	
	private Button(int button) {
		mButton = button;

	}
	
	public int waitForNextButtonPress() throws Exception {
		if (mButton == Button1 || mButton == Button2) {
			synchronized (this) {
				return waitForNextButtonPressNotSynched();
			}
		} else {
			synchronized (Button1()) {
				synchronized (Button2()) {
					return waitForNextButtonPressNotSynched();		
				}
			}
		}
	}
	
	private int waitForNextButtonPressNotSynched() throws Exception {
		UC.clearLastButtonPress(mButton);
		
		while (true) {
			int lastButtonPress = UC.getLastButtonPress(mButton);
			
			if (lastButtonPress == 0) {
				continue;
			}
						
			if (mButton == Both || mButton == lastButtonPress) {
				return lastButtonPress;
			}
		}
	}
	
	private static Button sButton1 = null;
	private static Button sButton2 = null;
	private static Button sBoth = null;

	public static synchronized Button Button1() {
		if (sButton1 == null) {
			sButton1 = new Button(Button1);
		}
		
		return sButton1;
	}
	
	public static synchronized Button Button2() {
		if (sButton2 == null) {
			sButton2 = new Button(Button2);
		}
		
		return sButton2;
	}
	
	public static synchronized Button Both() {
		if (sBoth == null) {
			sBoth = new Button(Both);
		}
		
		return sBoth;
	}
}
