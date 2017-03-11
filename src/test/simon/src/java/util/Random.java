package java.util;
import cc2650.UC;
import uj.lang.RT;

public class Random {
	public Random(int seed) {
		RT.srand(seed);
	}
	
	public Random() {
		this(UC.clkGet());
	}
	
	public int nextInt() {
		return RT.rand();
	}
	
	public int nextInt(int n) {	
        return nextInt() % n;
	}
}