package java.lang;
import uj.lang.*;

public class String extends MiniString{

	public String(byte[] bytes){	//our constructor for raw bytes
	
		super(bytes);
	}

	public static synchronized void print(String s){
		
		int i, L = s.Xlen_();
		
		for(i = 0; i < L; i++) uj.lang.RT.consolePut((char)s.XbyteAt_(i));	
	}
	
	public static synchronized void println(String s){
		String.print(s);
		String.print("\r\n");
	}
	
	public static String valueOf(int i) {
		return Integer.toString(i);
	}
}