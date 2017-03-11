package simon;

import cc2650.Flash;

public class HighScoreManager {
	private int mCurrentHighScore = 0;
	
	public HighScoreManager() {
		mCurrentHighScore = Flash.readInt(0);
		String.print("The current high score is: ");
		String.println(Integer.toString(mCurrentHighScore));
	}
	
	public int getHighScore() {
		return mCurrentHighScore;
	}
	
	public void updateHighScore(int score) {
		if (mCurrentHighScore > score) {
			String.println("Do better next time!");
			return;
		}
		
		if (mCurrentHighScore == score) {
			String.println("You are tied for the highest score!");
			return;
		}
		
		mCurrentHighScore = score;
		
		String.println("You have achieved the highest score!");
		
		Flash.erase(0, 4);
		Flash.writeInt(0, mCurrentHighScore);
	}
}
