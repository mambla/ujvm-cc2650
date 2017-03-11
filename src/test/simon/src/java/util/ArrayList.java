package java.util;

public class ArrayList {
	private int mCount;
	private int[] mElements;
	
	public ArrayList(int initialCapacity) {
		mElements = new int[initialCapacity];
		mCount = 0;
	}
	
	public ArrayList() {
		this(10);
	}
	
	public void set(int index, int element) throws Exception {
		if (index >= mCount) {
			throw new Exception();
		}
		
		mElements[index] = element;
	}
	
	public int get(int index) throws Exception {
		if (index >= mCount) {
			throw new Exception();
		}
		
		return mElements[index];
	}
	
	public void add(int e) {
		ensureCapacity(mCount + 1);  
		mElements[mCount++] = e;
	}
	
    public void ensureCapacity(int minCapacity) {
        int oldCapacity = mElements.length;
        
        if (minCapacity <= oldCapacity) {
        	return;
        }
        
        int[] oldData = mElements;
        int newCapacity = (oldCapacity * 3)/2 + 1;
        if (newCapacity < minCapacity) {
            newCapacity = minCapacity;
        }
        
        mElements = new int[newCapacity];
    	for (int i = 0; i < oldCapacity; ++i) {
    		mElements[i] = oldData[i];
    	}    	
    }

    public int size() {
        return mCount;
    }
}

