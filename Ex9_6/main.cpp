#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"
#include "adsr.h"
#include "iir_filter.h"

int main(void)
{
	MONO_PCM pcm0, pcm1;
	double a[3], b[3];

	pcm0.fs = 44100; /* 標本化周波数 */
	pcm0.bits = 16; /* 量子化精度 */
	pcm0.length = pcm0.fs * 4; /* 音データの長さ */
	pcm0.s = new double[pcm0.length](); /* 音データ */

	double vco = 440.0; /* 基本周波数 */

	/* ノコギリ波 */
	int t0 = pcm0.fs / vco; /* 基本周期 */
	int m = 0;
	for (int n = 0; n < pcm0.length; n++) {
		pcm0.s[n] = 1.0 - 2.0 * m / t0;

		m++;
		if (m >= t0) {
			m = 0;
		}
	}

	double* vcf = new double[pcm0.length]();  /* 遮断周波数 */
	int gate = pcm0.fs * 1;
	int duration = pcm0.fs * 4;
	int A = 0;
	int D = pcm0.fs * 1;
	double S = 0.0;
	int R = pcm0.fs * 1;
	ADSR(vcf, A, D, S, R, gate, duration);

	double offset = 500.0; /* 時間エンベロープのオフセット */
	double depth = 500.0; /* 時間エンベロープのデプス */
	for (int n = 0; n < pcm0.length; n++) {
		vcf[n] = offset + vcf[n] * depth;
	}

	double Q = 1 / sqrt(2.0); /* レゾナンス */
	int I = 2; /* 遅延器の数 */
	int J = 2; /* 遅延器の数 */

	pcm1.fs = pcm0.fs; /* 標本化周波数 */
	pcm1.bits = pcm0.bits; /* 量子化精度 */
	pcm1.length = pcm0.length; /* 音データの長さ */
	pcm1.s = new double[pcm1.length](); /* 音データ */

	/* フィルタリング */
	for (int n = 0; n < pcm1.length; n++) {
		IIR_LPF(vcf[n] / pcm1.fs, Q, a, b); /* IIRフィルタの設計 */

		for (m = 0; m <= J; m++) {
			if (n - m >= 0) {
				pcm1.s[n] += b[m] * pcm0.s[n - m];
			}
		}
		for (m = 1; m <= I; m++) {
			if (n - m >= 0) {
				pcm1.s[n] += -a[m] * pcm1.s[n - m];
			}
		}
	}

	double* vca = new double[pcm1.length](); /* 振幅 */
	gate = pcm0.fs * 4;
	duration = pcm0.fs * 4;
	A = 0;
	D = pcm0.fs * 4;
	S = 0.0;
	R = pcm0.fs * 1;
	ADSR(vca, A, D, S, R, gate, duration);

	double gain = 0.1; /* ゲイン */

	for (int n = 0; n < pcm1.length; n++) {
		pcm1.s[n] *= vca[n] * gain;
	}

	/* フェード処理 */
	for (int n = 0; n < pcm1.fs * 0.01; n++) {
		pcm1.s[n] *= (double)n / (pcm1.fs * 0.01);
		pcm1.s[pcm1.length - n - 1] *= (double)n / (pcm1.fs * 0.01);
	}

	wave_write_16bit_mono(&pcm1, "ex9_6.wav");

	delete[] pcm0.s;
	delete[] pcm1.s;
	delete[] vcf;
	delete[] vca;

	return 0;
}
