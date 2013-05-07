package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;

import com.mosync.api.CString;
import com.mosync.api.Pointer;
import com.mosync.api.CString.Storage;
import com.mosync.internal.android.extensions.IntType;

public abstract class PIMStringArrayField extends PIMField {

	byte[] getData(int index) {
		String[] val = getSpecificData(index);
		DebugPrint("DATA SIZE = " + getDataSize(val));
		byte[] buffer = new byte[getDataSize(val)];
		PIMUtil.writeStringArray(val, buffer);
		return buffer;
	}

	String[] getSpecificData(int index) {
		// Ehm... Why is there a -4 here???
		String[] val = mValues.get(index);
		String[] ret = new String[val.length - 4];
		for (int i = 0; i < val.length - 4; i++) {
			ret[i] = val[i + 1];
		}
		return ret;
	}

	int getDataSize(String[] val) {
		int size = IntType.getInstance().size();
		for (int i = 0; i < val.length; i++) {
			int strLen = val == null ? 0 : val[i].length();
			size += CString.getCharSize(Storage.WCHAR) * (strLen + 1);
		}
		return size;
	}

	void setData(int index, Pointer<Void> buffer) {
		String[] val = PIMUtil.readStringArray(buffer);
		setSpecificData(val, index);
	}

	void setSpecificData(String[] data, int index) {
		String[] val = mValues.get(index);
		for (int i = 0; i < data.length; i++) {
			val[i + 1] = data[i];
		}
		mValues.set(index, val);
	}
}
