package java.lang;
import cc2650.UC;

public class Thread implements Runnable{

	public Thread(Runnable what){
	
		uj.lang.RT.threadCreate(what);	
	}
	
	public void run(){
	
			
	}
	
	public static void sleep(int ms) {
		if (ms == 0) {
			return;
		}
		
		int end = UC.clkGet() + UC.clkSpeed() / 1000 * ms;
		
		while (UC.clkGet() < end) {
			
		}
	}
}