/**
* マリオの音の作成
*/

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "wave.h"
#include "sound_util.h"

using namespace std;
using namespace sutil;

int main(void)
{
	MONO_PCM pcm;

	pcm.fs = 44100; /* 標本化周波数 */
	pcm.bits = 16; /* 量子化精度 */
	pcm.length = 7000 * 16; /* 音データの長さ */
	pcm.s = new double[pcm.length]();

	vector<double> amplitude = { 0.1, 0.2, 0.1 };

	/* メロディパート */
	piano(&pcm, E5, amplitude[0], 7000 * 0, 6125 * 1);
	piano(&pcm, E5, amplitude[0], 7000 * 1, 6125 * 1);
	piano(&pcm, E5, amplitude[0], 7000 * 3, 6125 * 1);
	piano(&pcm, C5, amplitude[0], 7000 * 5, 6125 * 1);
	piano(&pcm, E5, amplitude[0], 7000 * 6, 6125 * 1);
	piano(&pcm, G5, amplitude[0], 7000 * 8, 6125 * 1);
	piano(&pcm, G4, amplitude[0], 7000 * 12, 6125 * 1);

	/* ベースパート */
	piano(&pcm, D3, amplitude[1], 7000 * 0, 6125 * 1);
	piano(&pcm, D3, amplitude[1], 7000 * 1, 6125 * 1);
	piano(&pcm, D3, amplitude[1], 7000 * 3, 6125 * 1);
	piano(&pcm, D3, amplitude[1], 7000 * 5, 6125 * 1);
	piano(&pcm, D3, amplitude[1], 7000 * 6, 6125 * 1);
	piano(&pcm, G3, amplitude[1], 7000 * 8, 6125 * 1);
	piano(&pcm, G3, amplitude[1], 7000 * 12, 6125 * 1);

	/* パーカッション */
	percussion(&pcm, amplitude[2], 0);
	percussion(&pcm, amplitude[2], 2, 0.25);
	percussion(&pcm, amplitude[2], 3);
	percussion(&pcm, amplitude[2], 5, 0.25);
	percussion(&pcm, amplitude[2], 6);
	percussion(&pcm, amplitude[2], 8);
	percussion(&pcm, amplitude[2], 11);
	percussion(&pcm, amplitude[2], 13, 0.25);
	percussion(&pcm, amplitude[2], 14, 0.25);
	percussion(&pcm, amplitude[2], 15, 0.25);

	wave_write_16bit_mono(&pcm, "ex8_6.wav");

	delete[] pcm.s;

	return 0;
}
