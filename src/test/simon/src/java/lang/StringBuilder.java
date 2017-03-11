package java.lang;

public class StringBuilder{

	private byte bytes[];

	public StringBuilder(){
	
		bytes = new byte[0];
	}
	
	public StringBuilder append(String s){
	
		int i, j;
		byte n[];
		
		if(s == null) s = "null";
		
		n = new byte[bytes.length + s.Xlen_()];
		for(i = 0; i < bytes.length; i++) n[i] = bytes[i];
		for(j = 0; j < s.Xlen_(); j++, i++) n[i] = s.XbyteAt_(j);
		
		bytes = n;
		
		return this;
	}
	
	public StringBuilder append(char c){
	
		byte str[];
		
		if(c <= 0x7F){
		
			str = new byte[1];
			str[0] = (byte)c;	
		}
		else if(c <= 0x7FF){
		
			str = new byte[2];
			str[1] = (byte)((c & 0x3F) | 0x80);
			c >>= 6;
			str[0] = (byte)(c | 0xC0);
		}
		else{
		
			str = new byte[2];
			str[2] = (byte)((c & 0x3F) | 0x80);
			c >>= 6;
			str[1] = (byte)((c & 0x3F) | 0x80);
			c >>= 6;
			str[0] = (byte)(c | 0xE0);
		}
	
		return append(new String(str));
	}
	
	public StringBuilder append(Object o){
	
		return append(o.toString());
	}
	
	public String toString(){
	
		return new String(bytes);
	}
}