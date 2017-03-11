package java.lang;

public class Integer {
	   
    public static String toString(int i) {
        byte buf[] = new byte[stringSize(i)];
        boolean negative = (i < 0);
        int charPos = buf.length - 1;

        if (negative) {
            i = -i;
        }

        while (i >= 10) {
            buf[charPos--] = (byte)((byte)'0' + (byte)(i % 10));
            i = i / 10;
        }
        
        buf[charPos] = (byte)((byte)'0' + (byte)i);

        if (negative) {
            buf[--charPos] = '-';
        }

        return new String(buf);
    }
    
    private static int stringSize(int x) {
    	final int [] sizeTable = { 9, 99, 999, 9999, 99999, 999999, 9999999, 99999999, 999999999, 0x7FFFFFFF };
    	
    	int unsigned = Math.abs(x);
    	
        for (int i=0; ; i++)
        {
            if (unsigned <= sizeTable[i])
            {
            	if (x >= 0) {
            		return i + 1;
            	} else {
            		return i + 2;
            	}
            }
       }
    }
}