/**
 * サイン波の作成
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

#ifndef M_PI
#define M_PI	3.14159265
#endif

int main(void)
{
	MONO_PCM pcm;

	pcm.fs = 44100; /* 標本化周波数 */
	pcm.bits = 16; /* 量子化精度 */
	pcm.length = pcm.fs * 1; /* 音データの長さ */
	pcm.s = new double[pcm.length](); /* 音データ */

	double a = 0.1; /* 振幅 */
	double f0 = 500.0; /* 周波数 */

	/* サイン波 */
	for (int n = 0; n < pcm.length; n++) {
		pcm.s[n] = a * sin(2.0 * M_PI * f0 * n / pcm.fs);
	}

	wave_write_16bit_mono(&pcm, "ex2_1.wav");

	delete[] pcm.s;

	return 0;
}
