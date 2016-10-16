/**
* 音の作成
*/

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "wave.h"
#include "sound_util.h"
#include "pitch_frequency.h"

using namespace std;
using namespace sutil;
using namespace pfreq;

int main(void)
{
	MONO_PCM pcm;

	pcm.fs = 44100; /* 標本化周波数 */
	pcm.bits = 16; /* 量子化精度 */
	pcm.length = 7000 * 48; /* 音データの長さ */
	pcm.s = new double[pcm.length]();

	vector<double> amplitude = { 0.1, 0.2, 0.1 };

	
	/* メロディパート */
	/*
	piano(&pcm, CN4, amplitude[0], 7000 * 0, 6125 * 3);
	piano(&pcm, DN4, amplitude[0], 7000 * 3, 6125 * 1);
	piano(&pcm, EN4, amplitude[0], 7000 * 4, 6125 * 3);
	piano(&pcm, CN4, amplitude[0], 7000 * 7, 6125 * 1);
	piano(&pcm, EN4, amplitude[0], 7000 * 8, 6125 * 2);
	piano(&pcm, CN4, amplitude[0], 7000 * 10, 6125 * 2);
	piano(&pcm, EN4, amplitude[0], 7000 * 12, 6125 * 3);

	flute(&pcm, DN4, amplitude[0], 7000 * 16, 6125 * 3);
	flute(&pcm, EN4, amplitude[0], 7000 * 19, 6125 * 1);
	flute(&pcm, FN4, amplitude[0], 7000 * 20, 6125 * 2);
	flute(&pcm, EN4, amplitude[0], 7000 * 22, 6125 * 1);
	flute(&pcm, DN4, amplitude[0], 7000 * 23, 6125 * 1);
	flute(&pcm, FN4, amplitude[0], 7000 * 24, 6125 * 7);

	violin(&pcm, EN4, amplitude[0], 7000 * 32, 6125 * 3);
	violin(&pcm, FN4, amplitude[0], 7000 * 35, 6125 * 1);
	violin(&pcm, GN4, amplitude[0], 7000 * 36, 6125 * 3);
	violin(&pcm, EN4, amplitude[0], 7000 * 39, 6125 * 1);
	violin(&pcm, GN4, amplitude[0], 7000 * 40, 6125 * 2);
	violin(&pcm, EN4, amplitude[0], 7000 * 42, 6125 * 2);
	violin(&pcm, GN4, amplitude[0], 7000 * 44, 6125 * 3);
	*/

	vector<int> pi12 = { 3, 1, 8, 4, 8, 0, 9, 4, 9, 3, 11,
		9, 1, 8, 6, 6, 4, 5, 7, 3, 10,
		6, 2, 1, 1, 11, 11, 1, 5, 1, 5,
		5, 1, 10, 0, 5, 7, 2, 9, 2, 9,
		0, 10, 7, 8, 0, 9, 10, 4, 9, 2,
		7, 4, 2, 1, 4, 0, 10, 6, 0, 10,
		5, 5, 2, 5, 6, 10, 0, 6, 6, 1,
		10, 0, 3, 7, 5, 3, 10, 3, 10, 10,
		5, 4, 8, 0, 5, 6, 4, 6, 8, 8,
		0, 1, 8, 1, 10, 3, 6, 8, 3, 0 };

	for (int i = 0; i < 48; i++) {	//To change max limit
		piano(&pcm, toHertz(pi12[i] + 40), amplitude[0], 7000 * i, 12250);
	}

	wave_write_16bit_mono(&pcm, "result.wav");

	delete[] pcm.s;

	return 0;
}
