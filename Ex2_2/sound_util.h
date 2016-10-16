#include <math.h>
#include <random>

namespace sutil {

	const double M_PI = 3.14159265;

	const double C2 = 65.41;
	const double D2 = 73.42;
	const double E2 = 82.41;
	const double F2 = 87.31;
	const double G2 = 98;
	const double A2 = 110;
	const double B2 = 123.47;

	const double C3 = 130.81;
	const double D3 = 146.83;
	const double E3 = 164.81;
	const double F3 = 174.61;
	const double G3 = 196;
	const double A3 = 220;
	const double B3 = 246.94;

	const double C4 = 261.63;
	const double D4 = 293.66;
	const double E4 = 329.63;
	const double F4 = 349.23;
	const double G4 = 392;
	const double A4 = 440;
	const double B4 = 493.88;

	const double C5 = 523.25;
	const double D5 = 587.33;
	const double E5 = 659.26;
	const double F5 = 698.46;
	const double G5 = 783.99;
	const double A5 = 880;
	const double B5 = 987.77;

	const double C6 = 1046.5;
	const double D6 = 1174.66;
	const double E6 = 1318.51;
	const double F6 = 1396.91;
	const double G6 = 1567.98;
	const double A6 = 1760;
	const double B6 = 1975.53;

	void sine_wave(MONO_PCM *pcm, double f0, double a, int offset, int duration) {
		double* s = new double[duration]();

		/* サイン波 */
		for (int n = 0; n < duration; n++) {
			s[n] = a * sin(2.0 * M_PI * f0 * n / pcm->fs);
		}

		/* フェード処理 */
		for (int n = 0; n < pcm->fs*0.01; n++) {
			s[n] *= (double)n / (pcm->fs * 0.01);
			s[duration - n - 1] *= (double)n / (pcm->fs * 0.01);
		}

		for (int n = 0; n < duration; n++) {
			pcm->s[offset + n] += s[n];
		}

		delete[] s;
	}

	void sawtooth_wave(MONO_PCM *pcm, double f0, double gain, int offset, int duration) {
		double* s = new double[duration]();

		/* のこぎり波 */
		int t0 = pcm->fs / f0; /* 基本周期 */
		int m = 0;
		for (int n = 0; n < duration; n++) {
			s[n] = -1.0 + 2.0 * m / t0;

			m++;
			if (m >= t0) {
				m = 0;
			}
		}

		for (int n = 0; n < duration; n++) {
			s[n] *= gain;
		}

		for (int n = 0; n < duration; n++) {
			pcm->s[offset + n] += s[n];
		}

		delete[] s;
	}

	void square_wave(MONO_PCM *pcm, double f0, double gain, int offset, int duration) {
		double* s = new double[duration]();

		/* 矩形波 */
		int t0 = pcm->fs / f0; /* 基本周期 */
		int m = 0;
		for (int n = 0; n < duration; n++) {
			if (m < t0 / 2.0) {
				s[n] = 1.0;
			}
			else {
				s[n] = -1.0;
			}

			m++;
			if (m >= t0) {
				m = 0;
			}
		}

		for (int n = 0; n < duration; n++) {
			s[n] *= gain;
		}

		for (int n = 0; n < duration; n++) {
			pcm->s[offset + n] += s[n];
		}

		delete[] s;
	}

	/**
	* 周期t0の範囲で、-gainからgainから-gainとなる三角波を生成する。
	*/
	void triangle_wave(MONO_PCM *pcm, double f0, double gain, int offset, int duration) {
		double* s = new double[duration]();

		/* 三角波 */
		int t0 = pcm->fs / f0; /* 基本周期 */
		int m = 0;
		for (int n = 0; n < duration; n++) {
			if (m < t0 / 2.0) {
				s[n] = -1.0 + 4.0 * m / t0;
			}
			else {
				s[n] = 3.0 - 4.0 * m / t0;
			}

			m++;
			if (m >= t0) {
				m = 0;
			}
		}

		for (int n = 0; n < duration; n++) {
			s[n] *= gain;
		}

		for (int n = 0; n < duration; n++) {
			pcm->s[offset + n] += s[n];
		}

		delete[] s;
	}

	void white_noise(MONO_PCM *pcm, double gain, int offset, int duration) {
		double* s = new double[duration]();

		/* 白色雑音 */
		for (int n = 0; n < duration; n++) {
			s[n] = (double)rand() / RAND_MAX * 2.0 - 1.0;
		}

		for (int n = 0; n < duration; n++) {
			s[n] *= gain;
		}

		for (int n = 0; n < duration; n++) {
			pcm->s[offset + n] += s[n];
		}

		delete[] s;
	}

	void piano(MONO_PCM *pcm, double f0, double gain, int offset, int duration) {
		double* s = new double[duration]();

		double gain_sub[10] = { 0.155, 0.108, 0.124, 0.108, 0.124, 0.131, 0.108, 0.108, 0.031, 0.004 };

		/* サイン波 */
		for (int k = 0; k < 10; ++k) {
			for (int n = 0; n < duration; n++) {
				double f = f0 * (k + 1);
				s[n] += gain * gain_sub[k] * sin(2.0 * M_PI * f * n / pcm->fs);
			}
		}

		/* フェード処理 */
		for (int n = 0; n < duration; ++n) {
			s[n] *= exp(-(double)n * 7.0 / pcm->fs);
		}

		for (int n = 0; n < duration; n++) {
			pcm->s[offset + n] += s[n];
		}

		delete[] s;
	}

	void percussion(MONO_PCM* pcm, double amplitude, double offset, double duration = 1) {
		white_noise(pcm, amplitude, 7000 * offset, 4000 * duration);
	}
}