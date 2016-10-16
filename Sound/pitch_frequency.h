#include <math.h>

namespace pfreq {
	//Converts from key number to Hertz (49=A=440Hz).
	double toHertz(int key) {
		return pow(pow(2, 1.0 / 12), key - 49) * 440;
	}

	/*
	Converts from Hertz to key number.
	Note: Non-integral vals are truncated.
	*/
	int toPitchID(double hertz) {
		return 12 * log2(hertz / 440.0) + 49;
	}
}