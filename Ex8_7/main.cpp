/**
* マリオでコインを取った時の音を作成
*/

#include <stdio.h>
#include <stdlib.h>
#include "wave.h"

int main(void)
{
	MONO_PCM pcm;

	pcm.fs = 44100; /* 標本化周波数 */
	pcm.bits = 16; /* 量子化精度 */
	pcm.length = pcm.fs * 0.6; /* 音データの長さ */
	pcm.s = new double[pcm.length](); /* 音データ */

	double* f0 = new double[pcm.length]();

	/* 基本周波数 */
	for (int n = 0; n < pcm.fs * 0.1; n++) {
		f0[n] = 987.77; /* B5 */
	}
	for (int n = pcm.fs * 0.1; n < pcm.length; n++) {
		f0[n] = 1318.51; /* E6 */
	}

	/* 矩形波 */
	int t0 = pcm.fs / f0[0]; /* 矩形波の基本周期 */
	int m = 0;
	for (int n = 0; n < pcm.length; n++) {
		if (m < t0 / 2.0) {
			pcm.s[n] = 1.0;
		}
		else {
			pcm.s[n] = -1.0;
		}

		m++;
		if (m >= t0) {
			t0 = pcm.fs / f0[n]; /* 矩形波の基本周期 */
			m = 0;
		}
	}

	double* e = new double[pcm.length]();

	int pe = pcm.length; /* 単調減少にかかる時間 */

	/* 時間エンベロープ */
	for (int n = 0; n < pcm.length; n++) {
		e[n] = 1.0 - (double)n / pe;
	}

	double gain = 0.1; /* ゲイン */

	for (int n = 0; n < pcm.length; n++) {
		pcm.s[n] *= e[n] * gain;
	}

	wave_write_16bit_mono(&pcm, "ex8_7.wav");

	delete[] pcm.s;
	delete[] f0;
	delete[] e;

	return 0;
}
