#include <math.h>
#include <random>

namespace sutil {

	const double M_PI = 3.14159265;

	const double CN2 = 65.41;
	const double DN2 = 73.42;
	const double EN2 = 82.41;
	const double FN2 = 87.31;
	const double GN2 = 98;
	const double AN2 = 110;
	const double BN2 = 123.47;

	const double CN3 = 130.81;
	const double DN3 = 146.83;
	const double EN3 = 164.81;
	const double FN3 = 174.61;
	const double GN3 = 196;
	const double AN3 = 220;
	const double BN3 = 246.94;

	const double CN4 = 261.63;
	const double DN4 = 293.66;
	const double EN4 = 329.63;
	const double FN4 = 349.23;
	const double GN4 = 392;
	const double AN4 = 440;
	const double BN4 = 493.88;

	const double CN5 = 523.25;
	const double DN5 = 587.33;
	const double EN5 = 659.26;
	const double FN5 = 698.46;
	const double GN5 = 783.99;
	const double AN5 = 880;
	const double BN5 = 987.77;

	const double CN6 = 1046.5;
	const double DN6 = 1174.66;
	const double EN6 = 1318.51;
	const double FN6 = 1396.91;
	const double GN6 = 1567.98;
	const double AN6 = 1760;
	const double BN6 = 1975.53;
	

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
		
		/* フェード処理（エンベロープ） */
		for (int n = 0; n < duration; ++n) {
			if (n < pcm->fs * 0.01) {
				s[n] *= (double)n / (pcm->fs * 0.01);
			}
			else {
				s[n] *= exp(-(double)(n - pcm->fs * 0.01) * 7.0 / pcm->fs);
			}
		}

		for (int n = 0; n < duration; n++) {
			pcm->s[offset + n] += s[n];
		}

		delete[] s;
	}

	void flute(MONO_PCM *pcm, double f0, double gain, int offset, int duration) {
		double* s = new double[duration]();

		double gain_sub[10] = { 0.678, 0.2035, 0.033, 0.067, 0.004, 0.002, 0.004, 0.005, 0.0006, 0 };

		/* サイン波 */
		for (int k = 0; k < 10; ++k) {
			for (int n = 0; n < duration; n++) {
				double f = f0 * (k + 1);
				s[n] += gain * gain_sub[k] * sin(2.0 * M_PI * f * n / pcm->fs);
			}
		}

		/* フェード処理（エンベロープ） */
		for (int n = 0; n < duration; ++n) {
			if (n < pcm->fs * 0.01) {
				s[n] *= (double)n / (pcm->fs * 0.01);
			}
			else {
				s[n] *= exp(-(double)(n - pcm->fs * 0.01) * 7.0 / pcm->fs);
			}
		}

		for (int n = 0; n < duration; n++) {
			pcm->s[offset + n] += s[n];
		}

		delete[] s;
	}

	void violin(MONO_PCM *pcm, double f0, double gain, int offset, int duration) {
		double* s = new double[duration]();

		double gain_sub[20] = { 0.25, 0.115, 0.11, 0.075, 0.0875, 0.1, 0.025, 0.055, 0.0325, 0.02, 0.0225, 0.025, 0.005, 0.0025, 0.005, 0.0025, 0.0025, 0.001, 0.001, 0.001 };

		/* サイン波 */
		for (int k = 0; k < 20; ++k) {
			for (int n = 0; n < duration; n++) {
				double f = f0 * (k + 1);
				s[n] += gain * gain_sub[k] * sin(2.0 * M_PI * f * n / pcm->fs);
			}
		}

		/* フェード処理（エンベロープ） */
		for (int n = 0; n < pcm->fs * 0.01; ++n) {
			if (n < pcm->fs * 0.01) {
				s[n] *= (double)n / (pcm->fs * 0.01);
			}
			else if (n < pcm->fs * 0.11) {
				s[n] *= (double)(pcm->fs * 0.11 - n) / (pcm->fs * 0.1) * 0.5 + 0.5;
			}
			else if (n < duration * 0.8) {
				s[n] *= 0.5;
			}
			else {
				s[n] *= exp(-(double)(n - duration * 0.8) * 7.0 / pcm->fs);
			}
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