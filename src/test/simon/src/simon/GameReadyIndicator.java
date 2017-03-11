package simon;

import cc2650.LED;

public class GameReadyIndicator implements Runnable {
	private boolean mGameReady;
	private boolean mRunFinished;
	
	public GameReadyIndicator() {
		mGameReady = false;
		mRunFinished = false;
	}
	
	@Override
	public void run() {
		while (!mGameReady) {
			LED.Red().flash(50);
			LED.Green().flash(50);
		}		
		
		mRunFinished = true;
	}
	
	public void setGameReady() {
		mGameReady = true;
		
		while (!mRunFinished) {}
	}
}