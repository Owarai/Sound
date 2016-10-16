#include <stdio.h>
#include <stdlib.h>
#include "wave.h"

int main(void)
{
	STEREO_PCM pcm0, pcm1;

	wave_read_16bit_stereo(&pcm0, "a.wav"); /* 音データの入力 */

	pcm1.fs = pcm0.fs; /* 標本化周波数 */
	pcm1.bits = pcm0.bits; /* 量子化精度 */
	pcm1.length = pcm0.length; /* 音データの長さ */
	pcm1.sL = new double[pcm1.length](); /* 音データ（左） */
	pcm1.sR = new double[pcm1.length](); /* 音データ（右） */

	for (int n = 0; n < pcm1.length; n++) {
		pcm1.sL[n] = pcm0.sL[n]; /* 音データ（左） のコピー */
		pcm1.sR[n] = pcm0.sR[n]; /* 音データ（右）のコピー */
	}

	wave_write_16bit_stereo(&pcm1, "b.wav"); /* 音データの出力 */

	delete pcm0.sL;
	delete pcm0.sR;
	delete pcm1.sL;
	delete pcm1.sR;

	return 0;
}
