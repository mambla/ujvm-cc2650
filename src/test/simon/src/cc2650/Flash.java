package cc2650;

public class Flash {
	public static int readInt(int offset) {
		int output = 0;
		
		byte[] intData = read(offset, 4);

		for (int i = intData.length - 1; i >= 0; --i) {
			output += intData[i];
			
			if (i > 0) {
				output = output << 8;
			}
		}

		return output;
	}
	
	public static void writeInt(int offset, int val) {
		byte[] intData = new byte[4];
		
		for (int i = 0; i < intData.length; ++i) {
			intData[i] = (byte)(val & 0xFF);
			val = val >> 8;		
		}
		
		write(offset, intData);
	}
	
	public static void write(int offset, byte[] data) {
		for (int i = 0; i < data.length; ++i) {
			UC.flashWrite(offset + i, data[i]);
		}
	}
	
	public static byte[] read(int offset, int size) {
		byte[] data = new byte[size];
		
		for (int i = 0; i < size; ++i) {
			data[i] = UC.flashRead(offset + i);
		}
		
		return data;
	}
	
	// Beware, writing anywhere that was written to before needs erasing, and erasing is done in 4k chunks!
	public static void erase(int offset, int size) {
		UC.flashErase(offset, size);
	}
}
