package java.lang;

public class Math {
	public static int max(int a, int b) {
		return a > b ? a : b;
	}
	
	public static int min(int a, int b) {
		return a < b ? a : b;
	}
	
	public static int abs(int num) {
		return num < 0 ? -num : num;
	}
}
