/**
* ピアノ音の作成
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
	pcm.length = pcm.fs * 4; /* 音データの長さ */
	pcm.s = new double[pcm.length](); /* 音データ */

	double* a0 = new double[pcm.length]();
	double* a1 = new double[pcm.length]();
	double* a2 = new double[pcm.length]();
	double* a3 = new double[pcm.length]();
	double* a4 = new double[pcm.length]();

	double* f0 = new double[pcm.length]();
	double* f1 = new double[pcm.length]();
	double* f2 = new double[pcm.length]();
	double* f3 = new double[pcm.length]();
	double* f4 = new double[pcm.length]();

	/* 時間エンベロープ */
	for (int n = 0; n < pcm.length; n++) {
		a0[n] = 1.0 * exp(-5.0 * n / (pcm.fs * 4.0));
		a1[n] = 0.8 * exp(-5.0 * n / (pcm.fs * 2.0));
		a2[n] = 0.6 * exp(-5.0 * n / (pcm.fs * 1.0));
		a3[n] = 0.5 * exp(-5.0 * n / (pcm.fs * 0.5));
		a4[n] = 0.4 * exp(-5.0 * n / (pcm.fs * 0.2));

		f0[n] = 440;
		f1[n] = 880;
		f2[n] = 1320;
		f3[n] = 1760;
		f4[n] = 2200;
	}

	/* 加算合成 */
	for (int n = 0; n < pcm.length; n++) {
		pcm.s[n] += a0[n] * sin(2.0 * M_PI * f0[n] * n / pcm.fs);
		pcm.s[n] += a1[n] * sin(2.0 * M_PI * f1[n] * n / pcm.fs);
		pcm.s[n] += a2[n] * sin(2.0 * M_PI * f2[n] * n / pcm.fs);
		pcm.s[n] += a3[n] * sin(2.0 * M_PI * f3[n] * n / pcm.fs);
		pcm.s[n] += a4[n] * sin(2.0 * M_PI * f4[n] * n / pcm.fs);
	}

	double gain = 0.1; /* ゲイン */

	for (int n = 0; n < pcm.length; n++) {
		pcm.s[n] *= gain;
	}

	/* フェード処理 */
	for (int n = 0; n < pcm.fs * 0.01; n++)	{
		pcm.s[n] *= (double)n / (pcm.fs * 0.01);
		pcm.s[pcm.length - n - 1] *= (double)n / (pcm.fs * 0.01);
	}

	wave_write_16bit_mono(&pcm, "ex5_5.wav");

	delete[] pcm.s;
	delete[] a0;
	delete[] a1;
	delete[] a2;
	delete[] a3;
	delete[] a4;
	delete[] f0;
	delete[] f1;
	delete[] f2;
	delete[] f3;
	delete[] f4;

	return 0;
}
