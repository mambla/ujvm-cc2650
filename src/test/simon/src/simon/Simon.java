package simon;
import java.util.ArrayList;
import java.util.Random;

import cc2650.Button;
import cc2650.GpioPin;
import cc2650.LED;

public class Simon {
	public static final int RedButton = Button.Button1;
	public static final int GreenButton = Button.Button2;
	
	private final GpioPin mRed;
	private final GpioPin mGreen;
	private final Random mRandom;
	private final HighScoreManager mHighScoreManager;
	
	public Simon() {
		String.println("Welcome to Simon");
		
		mRed = LED.Red();
		mGreen = LED.Green();
		mRandom = new Random();
		
		mHighScoreManager = new HighScoreManager();
	}
	
	public void startGame() throws Exception{	
		waitForGameReady();
		
		String.println("Starting a new game...");

		ArrayList pattern = new ArrayList();
		int stage = 1;
		
		while (true) {
			pattern.add(generateRandomSignal());
			
			playLedSequence(pattern, 100 + 1200 / (stage + 1), 100 + 400 / (stage + 1));
			
			if (!testPattern(pattern)) {
				signalFailure();
				break;
			}
			
			signalSuccess();
						
			String.print("Finished stage: ");
			String.println(Integer.toString(stage));
			Thread.sleep(500);
			stage++;
		}
		
		int score = stage - 1;
		String.print("Game Over! Your score is: ");
		String.println(Integer.toString(score));
		
		mHighScoreManager.updateHighScore(score);
	}
	
	private boolean testPattern(ArrayList pattern) throws Exception {
		for (int i = 0; i < pattern.size(); ++i) {
			int buttonPressed = Button.Both().waitForNextButtonPress();
			
			if (buttonPressed != pattern.get(i)) {
				return false;
			}
			
			if (buttonPressed == Button.Button1) {	
				LED.Red().flash(100);
			} else {
				LED.Green().flash(100);
			}
		}	
		
		return true;
	}
	
	private void waitForGameReady() throws Exception {
		String.println("Press any button to begin");
		
		GameReadyIndicator gameReadyIndicator = new GameReadyIndicator();
		
		new Thread(gameReadyIndicator);
		
		Button.Both().waitForNextButtonPress();
		
		gameReadyIndicator.setGameReady();
		
		Thread.sleep(1000);
	}
	
	private int generateRandomSignal() throws Exception {
		int randBool = mRandom.nextInt(2);
		
		if (randBool == 0) {
			return RedButton;
		} else {
			return GreenButton;
		}
	}
	
	private void signalSuccess() throws Exception {		
		Thread.sleep(500);
		mGreen.set(true);
		mRed.set(true);
		mGreen.flash(500);
		mRed.set(false);
		mGreen.set(false);
	}
	
	private void signalFailure() throws Exception {		
		Thread.sleep(500);
		mRed.flash(1000);
	}
	
	private void playLedSequence(ArrayList sequence, int shineMs, int intervalMs) throws Exception {
		for (int i = 0; i < sequence.size(); ++i)
		{
			if (sequence.get(i) == Simon.RedButton) {
				mRed.flash(intervalMs);
			} else {
				mGreen.flash(intervalMs);
			}

			if (i != sequence.size() - 1) {
				Thread.sleep(intervalMs);
			}
		}
	}	
}