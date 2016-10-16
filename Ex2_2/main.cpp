/**
* サイン波を使って、ドレミファソラシドを作成
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"
#include "sound_util.h"

using namespace sutil;

int main(void)
{
	MONO_PCM pcm;

	pcm.fs = 44100; /* 標本化周波数 */
	pcm.bits = 16; /* 量子化精度 */
	pcm.length = pcm.fs * 2; /* 音データの長さ */
	pcm.s = new double[pcm.length](); /* 音データ */

	triangle_wave(&pcm, C4, 0.1, pcm.fs * 0.00, pcm.fs * 0.25); /* C4 */
	triangle_wave(&pcm, D4, 0.1, pcm.fs * 0.25, pcm.fs * 0.25); /* D4 */
	triangle_wave(&pcm, E4, 0.1, pcm.fs * 0.50, pcm.fs * 0.25); /* E4 */
	triangle_wave(&pcm, F4, 0.1, pcm.fs * 0.75, pcm.fs * 0.25); /* F4 */
	triangle_wave(&pcm, G4, 0.1, pcm.fs * 1.00, pcm.fs * 0.25); /* G4 */
	triangle_wave(&pcm, A4, 0.1, pcm.fs * 1.25, pcm.fs * 0.25); /* A4 */
	triangle_wave(&pcm, B4, 0.1, pcm.fs * 1.50, pcm.fs * 0.25); /* B4 */
	triangle_wave(&pcm, C5, 0.1, pcm.fs * 1.75, pcm.fs * 0.25); /* C5 */
#if 0
	sine_wave(&pcm, C4, 0.1, pcm.fs * 0.00, pcm.fs * 0.25); /* C4 */
	sine_wave(&pcm, D4, 0.1, pcm.fs * 0.25, pcm.fs * 0.25); /* D4 */
	sine_wave(&pcm, E4, 0.1, pcm.fs * 0.50, pcm.fs * 0.25); /* E4 */
	sine_wave(&pcm, F4, 0.1, pcm.fs * 0.75, pcm.fs * 0.25); /* F4 */
	sine_wave(&pcm, G4, 0.1, pcm.fs * 1.00, pcm.fs * 0.25); /* G4 */
	sine_wave(&pcm, A4, 0.1, pcm.fs * 1.25, pcm.fs * 0.25); /* A4 */
	sine_wave(&pcm, B4, 0.1, pcm.fs * 1.50, pcm.fs * 0.25); /* B4 */
	sine_wave(&pcm, C5, 0.1, pcm.fs * 1.75, pcm.fs * 0.25); /* C5 */
#endif

	wave_write_16bit_mono(&pcm, "ex2_2.wav");

	delete[] pcm.s;

	return 0;
}
