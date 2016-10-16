/**
* 矩形波の作成
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

	double f0 = 500.0; /* 基本周波数 */

	/* 矩形波 */
	for (int i = 1; i <= 44; i = i + 2) {
		for (int n = 0; n < pcm.length; n++) {
			pcm.s[n] += 1.0 / i * sin(2.0 * M_PI * i * f0 * n / pcm.fs);
		}
	}

	double gain = 0.1; /* ゲイン */

	for (int n = 0; n < pcm.length; n++) {
		pcm.s[n] *= gain;
	}

	wave_write_16bit_mono(&pcm, "ex3_2.wav");

	delete[] pcm.s;

	return 0;
}
