typedef struct
{
	int fs; /* 標本化周波数 */
	int bits; /* 量子化精度 */
	int length; /* 音データの長さ */
	double *s; /* 音データ */
} MONO_PCM;

typedef struct
{
	int fs; /* 標本化周波数 */
	int bits; /* 量子化精度 */
	int length; /* 音データの長さ */
	double *sL; /* 音データ（Lチャンネル）*/
	double *sR; /* 音データ（Rチャンネル）*/
} STEREO_PCM;

void wave_read_8bit_mono(MONO_PCM *pcm, char *file_name) {
	FILE *fp;
	char riff_chunk_ID[4];
	long riff_chunk_size;
	char file_format_type[4];
	char fmt_chunk_ID[4];
	long fmt_chunk_size;
	short wave_format_type;
	short channel;
	long samples_per_sec;
	long bytes_per_sec;
	short block_size;
	short bits_per_sample;
	char data_chunk_ID[4];
	long data_chunk_size;

	fp = fopen(file_name, "rb");

	fread(riff_chunk_ID, 1, 4, fp);
	fread(&riff_chunk_size, 4, 1, fp);
	fread(file_format_type, 1, 4, fp);
	fread(fmt_chunk_ID, 1, 4, fp);
	fread(&fmt_chunk_size, 4, 1, fp);
	fread(&wave_format_type, 2, 1, fp);
	fread(&channel, 2, 1, fp);
	fread(&samples_per_sec, 4, 1, fp);
	fread(&bytes_per_sec, 4, 1, fp);
	fread(&block_size, 2, 1, fp);
	fread(&bits_per_sample, 2, 1, fp);
	fread(data_chunk_ID, 1, 4, fp);
	fread(&data_chunk_size, 4, 1, fp);

	pcm->fs = samples_per_sec; /* 標本化周波数 */
	pcm->bits = bits_per_sample; /* 量子化精度 */
	pcm->length = data_chunk_size; /* 音データの長さ */
	pcm->s = new double[pcm->length]; /* メモリの確保 */

	for (int n = 0; n < pcm->length; n++) {
		unsigned char data;
		fread(&data, 1, 1, fp); /* 音データの読み取り */
		pcm->s[n] = ((double)data - 128.0) / 128.0; /* 音データを-1以上1未満の範囲に正規化する */
	}

	fclose(fp);
}

void wave_write_8bit_mono(MONO_PCM *pcm, char *file_name) {
	FILE *fp;
	char riff_chunk_ID[4] = { 'R', 'I', 'F', 'F' };
	long riff_chunk_size = 36 + pcm->length;
	char file_format_type[4] = { ' W', 'A', 'V', 'E' };
	char fmt_chunk_ID[4] = { 'f', 'm', 't', ' ' };
	long fmt_chunk_size = 16;
	short wave_format_type = 1;
	short channel = 1;
	long samples_per_sec = pcm->fs; /* 標本化周波数 */
	long bytes_per_sec = pcm->fs * pcm->bits / 8;
	short block_size = pcm->bits / 8;
	short bits_per_sample = pcm->bits; /* 量子化精度 */
	char data_chunk_ID[4] = { 'd', 'a', 't', 'a' };
	long data_chunk_size = pcm->length;

	fp = fopen(file_name, "wb");

	fwrite(riff_chunk_ID, 1, 4, fp);
	fwrite(&riff_chunk_size, 4, 1, fp);
	fwrite(file_format_type, 1, 4, fp);
	fwrite(fmt_chunk_ID, 1, 4, fp);
	fwrite(&fmt_chunk_size, 4, 1, fp);
	fwrite(&wave_format_type, 2, 1, fp);
	fwrite(&channel, 2, 1, fp);
	fwrite(&samples_per_sec, 4, 1, fp);
	fwrite(&bytes_per_sec, 4, 1, fp);
	fwrite(&block_size, 2, 1, fp);
	fwrite(&bits_per_sample, 2, 1, fp);
	fwrite(data_chunk_ID, 1, 4, fp);
	fwrite(&data_chunk_size, 4, 1, fp);

	for (int n = 0; n < pcm->length; n++) {
		double s = (pcm->s[n] + 1.0) / 2.0 * 256.0;

		if (s > 255.0) {
			s = 255.0; /* クリッピング */
		}
		else if (s < 0.0) {
			s = 0.0; /* クリッピング */
		}

		unsigned char data = (unsigned char)((int)(s + 0.5)); /* 四捨五入 */
		fwrite(&data, 1, 1, fp); /* 音データの書き出し */
	}

	if ((pcm->length % 2) == 1) { /* 音データの長さが奇数のとき */
		unsigned char data = 0;
		fwrite(&data, 1, 1, fp); /* 0パディング */
	}

	fclose(fp);
}

void wave_read_8bit_stereo(STEREO_PCM *pcm, char *file_name) {
	FILE *fp;
	char riff_chunk_ID[4];
	long riff_chunk_size;
	char file_format_type[4];
	char fmt_chunk_ID[4];
	long fmt_chunk_size;
	short wave_format_type;
	short channel;
	long samples_per_sec;
	long bytes_per_sec;
	short block_size;
	short bits_per_sample;
	char data_chunk_ID[4];
	long data_chunk_size;

	fp = fopen(file_name, "rb");

	fread(riff_chunk_ID, 1, 4, fp);
	fread(&riff_chunk_size, 4, 1, fp);
	fread(file_format_type, 1, 4, fp);
	fread(fmt_chunk_ID, 1, 4, fp);
	fread(&fmt_chunk_size, 4, 1, fp);
	fread(&wave_format_type, 2, 1, fp);
	fread(&channel, 2, 1, fp);
	fread(&samples_per_sec, 4, 1, fp);
	fread(&bytes_per_sec, 4, 1, fp);
	fread(&block_size, 2, 1, fp);
	fread(&bits_per_sample, 2, 1, fp);
	fread(data_chunk_ID, 1, 4, fp);
	fread(&data_chunk_size, 4, 1, fp);

	pcm->fs = samples_per_sec; /* 標本化周波数 */
	pcm->bits = bits_per_sample; /* 量子化精度 */
	pcm->length = data_chunk_size / 2; /* 音データの長さ */
	pcm->sL = new double[pcm->length]; /* メモリの確保 */
	pcm->sR = new double[pcm->length]; /* メモリの確保 */

	for (int n = 0; n < pcm->length; n++) {
		unsigned char data;
		fread(&data, 1, 1, fp); /* 音データ（Lチャンネル）の読み取り */
		pcm->sL[n] = ((double)data - 128.0) / 128.0; /* 音データを-1以上1未満の範囲に正規化する */

		fread(&data, 1, 1, fp); /* 音データ（Rチャンネル）の読み取り */
		pcm->sR[n] = ((double)data - 128.0) / 128.0; /* 音データを-1以上1未満の範囲に正規化する */
	}

	fclose(fp);
}

void wave_write_8bit_stereo(STEREO_PCM *pcm, char *file_name) {
	FILE *fp;
	char riff_chunk_ID[4] = { 'R', 'I', 'F', 'F' };
	long riff_chunk_size = 36 + pcm->length * 2;
	char file_format_type[4] = { 'W', 'A', 'V', 'E' };
	char fmt_chunk_ID[4] = { 'f', 'm', 't', ' ' };
	long fmt_chunk_size = 16;
	short wave_format_type = 1;
	short channel = 2;
	long samples_per_sec = pcm->fs; /* 標本化周波数 */
	long bytes_per_sec = pcm->fs * pcm->bits / 8 * 2;
	short block_size = pcm->bits / 8 * 2;
	short bits_per_sample = pcm->bits; /* 量子化精度 */
	char data_chunk_ID[4] = { 'd', 'a', 't', 'a' };
	long data_chunk_size = pcm->length * 2;

	fp = fopen(file_name, "wb");

	fwrite(riff_chunk_ID, 1, 4, fp);
	fwrite(&riff_chunk_size, 4, 1, fp);
	fwrite(file_format_type, 1, 4, fp);
	fwrite(fmt_chunk_ID, 1, 4, fp);
	fwrite(&fmt_chunk_size, 4, 1, fp);
	fwrite(&wave_format_type, 2, 1, fp);
	fwrite(&channel, 2, 1, fp);
	fwrite(&samples_per_sec, 4, 1, fp);
	fwrite(&bytes_per_sec, 4, 1, fp);
	fwrite(&block_size, 2, 1, fp);
	fwrite(&bits_per_sample, 2, 1, fp);
	fwrite(data_chunk_ID, 1, 4, fp);
	fwrite(&data_chunk_size, 4, 1, fp);

	for (int n = 0; n < pcm->length; n++) {
		double sL = (pcm->sL[n] + 1.0) / 2.0 * 256.0;

		if (sL > 255.0) {
			sL = 255.0; /* クリッピング */
		}
		else if (sL < 0.0) {
			sL = 0.0; /* クリッピング */
		}

		unsigned char data = (unsigned char)((int)(sL + 0.5)); /* 四捨五入とオフセットの調節 */
		fwrite(&data, 1, 1, fp); /* 音データ（Lチャネル）の書き出し */

		double sR = (pcm->sR[n] + 1.0) / 2.0 * 256.0;

		if (sR > 255.0) {
			sR = 255.0; /* クリッピング */
		}
		else if (sR < 0.0) {
			sR = 0.0; /* クリッピング */
		}

		data = (unsigned char)((int)(sR + 0.5)); /* 四捨五入とオフセットの調節 */
		fwrite(&data, 1, 1, fp); /* 音データ（Rチャネル）の書き出し */
	}

	fclose(fp);
}

void wave_read_16bit_mono(MONO_PCM *pcm, char *file_name) {
	FILE *fp;
	char riff_chunk_ID[4];
	long riff_chunk_size;
	char file_format_type[4];
	char fmt_chunk_ID[4];
	long fmt_chunk_size;
	short wave_format_type;
	short channel;
	long samples_per_sec;
	long bytes_per_sec;
	short block_size;
	short bits_per_sample;
	char data_chunk_ID[4];
	long data_chunk_size;

	fp = fopen(file_name, "rb");

	fread(riff_chunk_ID, 1, 4, fp);
	fread(&riff_chunk_size, 4, 1, fp);
	fread(file_format_type, 1, 4, fp);
	fread(fmt_chunk_ID, 1, 4, fp);
	fread(&fmt_chunk_size, 4, 1, fp);
	fread(&wave_format_type, 2, 1, fp);
	fread(&channel, 2, 1, fp);
	fread(&samples_per_sec, 4, 1, fp);
	fread(&bytes_per_sec, 4, 1, fp);
	fread(&block_size, 2, 1, fp);
	fread(&bits_per_sample, 2, 1, fp);
	fread(data_chunk_ID, 1, 4, fp);
	fread(&data_chunk_size, 4, 1, fp);

	pcm->fs = samples_per_sec; /* 標本化周波数 */
	pcm->bits = bits_per_sample; /* 量子化精度 */
	pcm->length = data_chunk_size / 2; /* 音データの長さ */
	pcm->s = new double[pcm->length]; /* メモリの確保 */

	for (int n = 0; n < pcm->length; n++) {
		short data;
		fread(&data, 2, 1, fp); /* 音データの読み取り */
		pcm->s[n] = (double)data / 32768.0; /* 音データを-1以上1未満の範囲に正規化する */
	}

	fclose(fp);
}

void wave_write_16bit_mono(MONO_PCM *pcm, char *file_name) {
	FILE *fp;
	char riff_chunk_ID[4] = { 'R', 'I', 'F', 'F' };
	long riff_chunk_size = 36 + pcm->length * 2;
	char file_format_type[4] = { 'W', 'A', 'V', 'E' };
	char fmt_chunk_ID[4] = { 'f', 'm', 't', ' ' };
	long fmt_chunk_size = 16;
	short wave_format_type = 1;
	short channel = 1;
	long samples_per_sec = pcm->fs; /* 標本化周波数 */
	long bytes_per_sec = pcm->fs * pcm->bits / 8;
	short block_size = pcm->bits / 8;
	short bits_per_sample = pcm->bits; /* 量子化精度 */
	char data_chunk_ID[4] = { 'd', 'a', 't', 'a' };
	long data_chunk_size = pcm->length * 2;

	fp = fopen(file_name, "wb");

	fwrite(riff_chunk_ID, 1, 4, fp);
	fwrite(&riff_chunk_size, 4, 1, fp);
	fwrite(file_format_type, 1, 4, fp);
	fwrite(fmt_chunk_ID, 1, 4, fp);
	fwrite(&fmt_chunk_size, 4, 1, fp);
	fwrite(&wave_format_type, 2, 1, fp);
	fwrite(&channel, 2, 1, fp);
	fwrite(&samples_per_sec, 4, 1, fp);
	fwrite(&bytes_per_sec, 4, 1, fp);
	fwrite(&block_size, 2, 1, fp);
	fwrite(&bits_per_sample, 2, 1, fp);
	fwrite(data_chunk_ID, 1, 4, fp);
	fwrite(&data_chunk_size, 4, 1, fp);

	for (int n = 0; n < pcm->length; n++) {
		double s = (pcm->s[n] + 1.0) / 2.0 * 65536.0;

		if (s > 65535.0) {
			s = 65535.0; /* クリッピング */
		}
		else if (s < 0.0) {
			s = 0.0; /* クリッピング */
		}

		short data = (short)((int)(s + 0.5) - 32768); /* 四捨五入とオフセットの調節 */
		fwrite(&data, 2, 1, fp); /* 音データの書き出し */
	}

	fclose(fp);
}

void wave_read_16bit_stereo(STEREO_PCM *pcm, char *file_name) {
	FILE *fp;
	char riff_chunk_ID[4];
	long riff_chunk_size;
	char file_format_type[4];
	char fmt_chunk_ID[4];
	long fmt_chunk_size;
	short wave_format_type;
	short channel;
	long samples_per_sec;
	long bytes_per_sec;
	short block_size;
	short bits_per_sample;
	char data_chunk_ID[4];
	long data_chunk_size;

	fp = fopen(file_name, "rb");

	fread(riff_chunk_ID, 1, 4, fp);
	fread(&riff_chunk_size, 4, 1, fp);
	fread(file_format_type, 1, 4, fp);
	fread(fmt_chunk_ID, 1, 4, fp);
	fread(&fmt_chunk_size, 4, 1, fp);
	fread(&wave_format_type, 2, 1, fp);
	fread(&channel, 2, 1, fp);
	fread(&samples_per_sec, 4, 1, fp);
	fread(&bytes_per_sec, 4, 1, fp);
	fread(&block_size, 2, 1, fp);
	fread(&bits_per_sample, 2, 1, fp);
	fread(data_chunk_ID, 1, 4, fp);
	fread(&data_chunk_size, 4, 1, fp);

	pcm->fs = samples_per_sec; /* 標本化周波数 */
	pcm->bits = bits_per_sample; /* 量子化精度 */
	pcm->length = data_chunk_size / 4; /* 音データの長さ */
	pcm->sL = new double[pcm->length]; /* メモリの確保 */
	pcm->sR = new double[pcm->length]; /* メモリの確保 */

	for (int n = 0; n < pcm->length; n++) {
		short data;
		fread(&data, 2, 1, fp); /* 音データ（Lチャンネル）の読み取り */
		pcm->sL[n] = (double)data / 32768.0; /* 音データを-1以上1未満の範囲に正規化する */

		fread(&data, 2, 1, fp); /* 音データ（Rチャンネル）の読み取り */
		pcm->sR[n] = (double)data / 32768.0; /* 音データを-1以上1未満の範囲に正規化する */
	}

	fclose(fp);
}

void wave_write_16bit_stereo(STEREO_PCM *pcm, char *file_name) {
	FILE *fp;
	char riff_chunk_ID[4] = { 'R', 'I', 'F', 'F' };
	long riff_chunk_size = 36 + pcm->length * 4;
	char file_format_type[4] = { 'W', 'A', 'V', 'E' };
	char fmt_chunk_ID[4] = { 'f', 'm', 't', ' ' };
	long fmt_chunk_size = 16;
	short wave_format_type = 1;
	short channel = 2;
	long samples_per_sec = pcm->fs; /* 標本化周波数 */
	long bytes_per_sec = pcm->fs * pcm->bits / 8 * 2;
	short block_size = pcm->bits / 8 * 2;
	short bits_per_sample = pcm->bits; /* 量子化精度 */
	char data_chunk_ID[4] = { 'd', 'a', 't', 'a' };
	long data_chunk_size = pcm->length * 4;

	fp = fopen(file_name, "wb");

	fwrite(riff_chunk_ID, 1, 4, fp);
	fwrite(&riff_chunk_size, 4, 1, fp);
	fwrite(file_format_type, 1, 4, fp);
	fwrite(fmt_chunk_ID, 1, 4, fp);
	fwrite(&fmt_chunk_size, 4, 1, fp);
	fwrite(&wave_format_type, 2, 1, fp);
	fwrite(&channel, 2, 1, fp);
	fwrite(&samples_per_sec, 4, 1, fp);
	fwrite(&bytes_per_sec, 4, 1, fp);
	fwrite(&block_size, 2, 1, fp);
	fwrite(&bits_per_sample, 2, 1, fp);
	fwrite(data_chunk_ID, 1, 4, fp);
	fwrite(&data_chunk_size, 4, 1, fp);

	for (int n = 0; n < pcm->length; n++) {
		double sL = (pcm->sL[n] + 1.0) / 2.0 * 65536.0;

		if (sL > 65535.0) {
			sL = 65535.0; /* クリッピング */
		}
		else if (sL < 0.0) {
			sL = 0.0; /* クリッピング */
		}

		short data = (short)((int)(sL + 0.5) - 32768); /* ŽlŽÌŒÜ“ü‚ÆƒIƒtƒZƒbƒg‚Ì’²ß */
		fwrite(&data, 2, 1, fp); /* ‰¹ƒf[ƒ^iLƒ`ƒƒƒ“ƒlƒ‹j‚Ì‘‚«o‚µ */

		double sR = (pcm->sR[n] + 1.0) / 2.0 * 65536.0;

		if (sR > 65535.0) {
			sR = 65535.0; /* クリッピング */
		}
		else if (sR < 0.0) {
			sR = 0.0; /* クリッピング */
		}

		data = (short)((int)(sR + 0.5) - 32768); /* ŽlŽÌŒÜ“ü‚ÆƒIƒtƒZƒbƒg‚Ì’²ß */
		fwrite(&data, 2, 1, fp); /* ‰¹ƒf[ƒ^iRƒ`ƒƒƒ“ƒlƒ‹j‚Ì‘‚«o‚µ */
	}

	fclose(fp);
}

void wave_read_PCMU_mono(MONO_PCM *pcm, char *file_name) {
	FILE *fp;
	char riff_chunk_ID[4];
	long riff_chunk_size;
	char file_format_type[4];
	char fmt_chunk_ID[4];
	long fmt_chunk_size;
	short wave_format_type;
	short channel;
	long samples_per_sec;
	long bytes_per_sec;
	short block_size;
	short bits_per_sample;
	short extra_size;
	char fact_chunk_ID[4];
	long fact_chunk_size;
	long sample_length;
	char data_chunk_ID[4];
	long data_chunk_size;

	short s; /* 16bit‚Ì‰¹ƒf[ƒ^ */
	unsigned char sign, exponent, mantissa;
	int magnitude;

	fp = fopen(file_name, "rb");

	fread(riff_chunk_ID, 1, 4, fp);
	fread(&riff_chunk_size, 4, 1, fp);
	fread(file_format_type, 1, 4, fp);
	fread(fmt_chunk_ID, 1, 4, fp);
	fread(&fmt_chunk_size, 4, 1, fp);
	fread(&wave_format_type, 2, 1, fp);
	fread(&channel, 2, 1, fp);
	fread(&samples_per_sec, 4, 1, fp);
	fread(&bytes_per_sec, 4, 1, fp);
	fread(&block_size, 2, 1, fp);
	fread(&bits_per_sample, 2, 1, fp);
	fread(&extra_size, 2, 1, fp);
	fread(fact_chunk_ID, 1, 4, fp);
	fread(&fact_chunk_size, 4, 1, fp);
	fread(&sample_length, 4, 1, fp);
	fread(data_chunk_ID, 1, 4, fp);
	fread(&data_chunk_size, 4, 1, fp);

	pcm->fs = samples_per_sec; /* •W–{‰»Žü”g” */
	pcm->bits = 16; /* —ÊŽq‰»¸“x */
	pcm->length = sample_length; /* ‰¹ƒf[ƒ^‚Ì’·‚³ */
	pcm->s = new double[pcm->length]; /* ƒƒ‚ƒŠ‚ÌŠm•Û */

	for (int n = 0; n < pcm->length; n++) {
		unsigned char c;
		fread(&c, 1, 1, fp); /* ˆ³kƒf[ƒ^‚Ì“Ç‚ÝŽæ‚è */

		c = ~c;

		sign = c & 0x80;
		exponent = (c >> 4) & 0x07;
		mantissa = c & 0x0F;

		magnitude = ((((int)mantissa << 3) + 0x84) << exponent) - 0x84;

		if (sign == 0x80) {
			s = -(short)magnitude;
		}
		else {
			s = (short)magnitude;
		}

		pcm->s[n] = (double)s / 32768.0; /* ‰¹ƒf[ƒ^‚ð-1ˆÈã1–¢–ž‚Ì”ÍˆÍ‚É³‹K‰»‚·‚é */
	}

	fclose(fp);
}

void wave_write_PCMU_mono(MONO_PCM *pcm, char *file_name) {
	FILE *fp;
	char riff_chunk_ID[4] = { 'R', 'I', 'F', 'F' };
	long riff_chunk_size = 50 + pcm->length;
	char file_format_type[4] = { 'W', 'A', 'V', 'E' };
	char fmt_chunk_ID[4] = { 'f', 'm', 't', ' ' };
	long fmt_chunk_size = 18;
	short wave_format_type = 7;
	short channel = 1;
	long samples_per_sec = pcm->fs; /* •W–{‰»Žü”g” */
	long bytes_per_sec = samples_per_sec;
	short block_size = 1;
	short bits_per_sample = 8; /* —ÊŽq‰»¸“x */
	short extra_size = 0;
	char fact_chunk_ID[4] = { 'f', 'a', 'c', 't' };
	long fact_chunk_size = 4;
	long sample_length = pcm->length;
	char data_chunk_ID[4] = { 'd', 'a', 't', 'a' };
	long data_chunk_size = pcm->length;

	unsigned char sign, exponent, mantissa;
	int n, magnitude;

	static short level[8] =
	{
		0x00FF, 0x01FF, 0x03FF, 0x07FF, 0x0FFF, 0x1FFF, 0x3FFF, 0x7FFF
	};

	fp = fopen(file_name, "wb");

	fwrite(riff_chunk_ID, 1, 4, fp);
	fwrite(&riff_chunk_size, 4, 1, fp);
	fwrite(file_format_type, 1, 4, fp);
	fwrite(fmt_chunk_ID, 1, 4, fp);
	fwrite(&fmt_chunk_size, 4, 1, fp);
	fwrite(&wave_format_type, 2, 1, fp);
	fwrite(&channel, 2, 1, fp);
	fwrite(&samples_per_sec, 4, 1, fp);
	fwrite(&bytes_per_sec, 4, 1, fp);
	fwrite(&block_size, 2, 1, fp);
	fwrite(&bits_per_sample, 2, 1, fp);
	fwrite(&extra_size, 2, 1, fp);
	fwrite(fact_chunk_ID, 1, 4, fp);
	fwrite(&fact_chunk_size, 4, 1, fp);
	fwrite(&sample_length, 4, 1, fp);
	fwrite(data_chunk_ID, 1, 4, fp);
	fwrite(&data_chunk_size, 4, 1, fp);

	for (n = 0; n < pcm->length; n++) {
		double x = (pcm->s[n] + 1.0) / 2.0 * 65536.0;

		if (x > 65535.0) 		{
			x = 65535.0; /* クリッピング */
		}
		else if (x < 0.0) {
			x = 0.0; /* クリッピング */
		}

		short s = (short)((int)(x + 0.5) - 32768); /* ŽlŽÌŒÜ“ü‚ÆƒIƒtƒZƒbƒg‚Ì’²ß */

		if (s < 0) {
			magnitude = -s;
			sign = 0x80;
		}
		else {
			magnitude = s;
			sign = 0x00;
		}

		magnitude += 0x84;
		if (magnitude > 0x7FFF)	{
			magnitude = 0x7FFF;
		}

		for (exponent = 0; exponent < 8; exponent++) {
			if (magnitude <= level[exponent]) {
				break;
			}
		}

		mantissa = (magnitude >> (exponent + 3)) & 0x0F;

		unsigned char c = ~(sign | (exponent << 4) | mantissa);

		fwrite(&c, 1, 1, fp); /* ˆ³kƒf[ƒ^‚Ì‘‚«o‚µ */
	}

	if ((pcm->length % 2) == 1) { /* ˆ³kƒf[ƒ^‚Ì’·‚³‚ªŠï”‚Ì‚Æ‚« */
		unsigned char c = 0;
		fwrite(&c, 1, 1, fp); /* 0ƒpƒfƒBƒ“ƒO */
	}

	fclose(fp);
}

void wave_read_PCMA_mono(MONO_PCM *pcm, char *file_name) {
	FILE *fp;
	char riff_chunk_ID[4];
	long riff_chunk_size;
	char file_format_type[4];
	char fmt_chunk_ID[4];
	long fmt_chunk_size;
	short wave_format_type;
	short channel;
	long samples_per_sec;
	long bytes_per_sec;
	short block_size;
	short bits_per_sample;
	short extra_size;
	char fact_chunk_ID[4];
	long fact_chunk_size;
	long sample_length;
	char data_chunk_ID[4];
	long data_chunk_size;

	short s; /* 16bit‚Ì‰¹ƒf[ƒ^ */
	unsigned char sign, exponent, mantissa;
	int magnitude;

	fp = fopen(file_name, "rb");

	fread(riff_chunk_ID, 1, 4, fp);
	fread(&riff_chunk_size, 4, 1, fp);
	fread(file_format_type, 1, 4, fp);
	fread(fmt_chunk_ID, 1, 4, fp);
	fread(&fmt_chunk_size, 4, 1, fp);
	fread(&wave_format_type, 2, 1, fp);
	fread(&channel, 2, 1, fp);
	fread(&samples_per_sec, 4, 1, fp);
	fread(&bytes_per_sec, 4, 1, fp);
	fread(&block_size, 2, 1, fp);
	fread(&bits_per_sample, 2, 1, fp);
	fread(&extra_size, 2, 1, fp);
	fread(fact_chunk_ID, 1, 4, fp);
	fread(&fact_chunk_size, 4, 1, fp);
	fread(&sample_length, 4, 1, fp);
	fread(data_chunk_ID, 1, 4, fp);
	fread(&data_chunk_size, 4, 1, fp);

	pcm->fs = samples_per_sec; /* •W–{‰»Žü”g” */
	pcm->bits = 16; /* —ÊŽq‰»¸“x */
	pcm->length = sample_length; /* ‰¹ƒf[ƒ^‚Ì’·‚³ */
	pcm->s = new double[pcm->length]; /* ƒƒ‚ƒŠ‚ÌŠm•Û */

	for (int n = 0; n < pcm->length; n++) {
		unsigned char c;
		fread(&c, 1, 1, fp); /* ˆ³kƒf[ƒ^‚Ì“Ç‚ÝŽæ‚è */

		c ^= 0xD5;

		sign = c & 0x80;
		exponent = (c >> 4) & 0x07;
		mantissa = c & 0x0F;

		if (exponent == 0) {
			magnitude = ((int)mantissa << 4) + 0x0008;
		}
		else {
			magnitude = (((int)mantissa << 4) + 0x0108) << (exponent - 1);
		}

		if (sign == 0x80) {
			s = -(short)magnitude;
		}
		else {
			s = (short)magnitude;
		}

		pcm->s[n] = (double)s / 32768.0; /* ‰¹ƒf[ƒ^‚ð-1ˆÈã1–¢–ž‚Ì”ÍˆÍ‚É³‹K‰»‚·‚é */
	}

	fclose(fp);
}

void wave_write_PCMA_mono(MONO_PCM *pcm, char *file_name) {
	FILE *fp;
	char riff_chunk_ID[4] = { 'R', 'I', 'F', 'F' };
	long riff_chunk_size = 50 + pcm->length;
	char file_format_type[4] = { 'W', 'A', 'V', 'E' };
	char fmt_chunk_ID[4] = { 'f', 'm', 't', ' ' };
	long fmt_chunk_size = 18;
	short wave_format_type = 6;
	short channel = 1;
	long samples_per_sec = pcm->fs; /* •W–{‰»Žü”g” */
	long bytes_per_sec = samples_per_sec;
	short block_size = 1;
	short bits_per_sample = 8; /* —ÊŽq‰»¸“x */
	short extra_size = 0;
	char fact_chunk_ID[4] = { 'f', 'a', 'c', 't' };
	long fact_chunk_size = 4;
	long sample_length = pcm->length;
	char data_chunk_ID[4] = { 'd', 'a', 't', 'a' };
	long data_chunk_size = pcm->length;

	unsigned char sign, exponent, mantissa;
	int magnitude;

	static int level[8] =
	{
		0x00FF, 0x01FF, 0x03FF, 0x07FF, 0x0FFF, 0x1FFF, 0x3FFF, 0x7FFF
	};

	fp = fopen(file_name, "wb");

	fwrite(riff_chunk_ID, 1, 4, fp);
	fwrite(&riff_chunk_size, 4, 1, fp);
	fwrite(file_format_type, 1, 4, fp);
	fwrite(fmt_chunk_ID, 1, 4, fp);
	fwrite(&fmt_chunk_size, 4, 1, fp);
	fwrite(&wave_format_type, 2, 1, fp);
	fwrite(&channel, 2, 1, fp);
	fwrite(&samples_per_sec, 4, 1, fp);
	fwrite(&bytes_per_sec, 4, 1, fp);
	fwrite(&block_size, 2, 1, fp);
	fwrite(&bits_per_sample, 2, 1, fp);
	fwrite(&extra_size, 2, 1, fp);
	fwrite(fact_chunk_ID, 1, 4, fp);
	fwrite(&fact_chunk_size, 4, 1, fp);
	fwrite(&sample_length, 4, 1, fp);
	fwrite(data_chunk_ID, 1, 4, fp);
	fwrite(&data_chunk_size, 4, 1, fp);

	for (int n = 0; n < pcm->length; n++) {
		double x = (pcm->s[n] + 1.0) / 2.0 * 65536.0;

		if (x > 65535.0) {
			x = 65535.0; /* クリッピング */
		}
		else if (x < 0.0) {
			x = 0.0; /* クリッピング */
		}

		short s = (short)((int)(x + 0.5) - 32768); /* ŽlŽÌŒÜ“ü‚ÆƒIƒtƒZƒbƒg‚Ì’²ß */

		if (s < 0) {
			magnitude = -s;
			sign = 0x80;
		}
		else {
			magnitude = s;
			sign = 0x00;
		}

		if (magnitude > 0x7FFF) {
			magnitude = 0x7FFF;
		}

		for (exponent = 0; exponent < 8; exponent++) {
			if (magnitude <= level[exponent]) {
				break;
			}
		}

		if (exponent == 0) {
			mantissa = (magnitude >> 4) & 0x0F;
		}
		else {
			mantissa = (magnitude >> (exponent + 3)) & 0x0F;
		}

		unsigned char c = (sign | (exponent << 4) | mantissa) ^ 0xD5;

		fwrite(&c, 1, 1, fp); /* ˆ³kƒf[ƒ^‚Ì‘‚«o‚µ */
	}

	if ((pcm->length % 2) == 1) { /* ˆ³kƒf[ƒ^‚Ì’·‚³‚ªŠï”‚Ì‚Æ‚« */
		unsigned char c = 0;
		fwrite(&c, 1, 1, fp); /* 0ƒpƒfƒBƒ“ƒO */
	}

	fclose(fp);
}

void wave_read_IMA_ADPCM_mono(MONO_PCM *pcm, char *file_name) {
	FILE *fp;
	char riff_chunk_ID[4];
	long riff_chunk_size;
	char file_format_type[4];
	char fmt_chunk_ID[4];
	long fmt_chunk_size;
	short wave_format_type;
	short channel;
	long samples_per_sec;
	long bytes_per_sec;
	short block_size;
	short bits_per_sample;
	short extra_size;
	short samples_per_block;
	char fact_chunk_ID[4];
	long fact_chunk_size;
	long sample_length;
	char data_chunk_ID[4];
	long data_chunk_size;

	short s; /* 16bit‚Ì‰¹ƒf[ƒ^ */
	unsigned char c; /* 4bit‚Ìˆ³kƒf[ƒ^ */
	unsigned char header[4];
	unsigned char data;
	int n, sp, dp, offset, block, number_of_block, index, step_size;

	static int index_table[16] =
	{
		-1, -1, -1, -1, 2, 4, 6, 8,
		-1, -1, -1, -1, 2, 4, 6, 8
	};

	static int step_size_table[89] =
	{
		7, 8, 9, 10, 11, 12, 13, 14,
		16, 17, 19, 21, 23, 25, 28, 31,
		34, 37, 41, 45, 50, 55, 60, 66,
		73, 80, 88, 97, 107, 118, 130, 143,
		157, 173, 190, 209, 230, 253, 279, 307,
		337, 371, 408, 449, 494, 544, 598, 658,
		724, 796, 876, 963, 1060, 1166, 1282, 1411,
		1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024,
		3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484,
		7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
		15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794,
		32767
	};

	fp = fopen(file_name, "rb");

	fread(riff_chunk_ID, 1, 4, fp);
	fread(&riff_chunk_size, 4, 1, fp);
	fread(file_format_type, 1, 4, fp);
	fread(fmt_chunk_ID, 1, 4, fp);
	fread(&fmt_chunk_size, 4, 1, fp);
	fread(&wave_format_type, 2, 1, fp);
	fread(&channel, 2, 1, fp);
	fread(&samples_per_sec, 4, 1, fp);
	fread(&bytes_per_sec, 4, 1, fp);
	fread(&block_size, 2, 1, fp);
	fread(&bits_per_sample, 2, 1, fp);
	fread(&extra_size, 2, 1, fp);
	fread(&samples_per_block, 2, 1, fp);
	fread(fact_chunk_ID, 1, 4, fp);
	fread(&fact_chunk_size, 4, 1, fp);
	fread(&sample_length, 4, 1, fp);
	fread(data_chunk_ID, 1, 4, fp);
	fread(&data_chunk_size, 4, 1, fp);

	number_of_block = data_chunk_size / block_size;

	pcm->fs = samples_per_sec; /* •W–{‰»Žü”g” */
	pcm->bits = 16; /* —ÊŽq‰»¸“x */
	pcm->length = sample_length; /* ‰¹ƒf[ƒ^‚Ì’·‚³ */
	pcm->s = (double*)calloc(pcm->length, sizeof(double)); /* ƒƒ‚ƒŠ‚ÌŠm•Û */

	for (block = 0; block < number_of_block; block++) {
		offset = samples_per_block * block;

		for (n = 0; n < samples_per_block; n++)	{
			if (n == 0)	{
				fread(header, 1, 4, fp); /* ƒwƒbƒ_‚Ì“Ç‚ÝŽæ‚è */

				sp = ((short)(char)header[1] << 8) + header[0];
				index = header[2];

				s = sp;
			}
			else {
				if ((n % 2) == 1) {
					fread(&data, 1, 1, fp); /* ˆ³kƒf[ƒ^‚Ì“Ç‚ÝŽæ‚è */

					c = (unsigned char)(data & 0x0F); /* data‚Ì‰ºˆÊ4bit */
				}
				else {
					c = (unsigned char)((data >> 4) & 0x0F); /* data‚ÌãˆÊ4bit */
				}

				step_size = step_size_table[index];

				/* L’£ */
				dp = step_size >> 3;
				if ((c & 0x1) == 0x1) {
					dp += (step_size >> 2);
				}
				if ((c & 0x2) == 0x2) {
					dp += (step_size >> 1);
				}
				if ((c & 0x4) == 0x4) {
					dp += step_size;
				}

				if ((c & 0x8) == 0x8) {
					sp -= dp;
				}
				else {
					sp += dp;
				}

				if (sp > 32767)	{
					sp = 32767;
				}
				else if (sp < -32768) {
					sp = -32768;
				}

				index += index_table[c];
				if (index < 0) {
					index = 0;
				}
				else if (index > 88) {
					index = 88;
				}

				s = sp;
			}

			pcm->s[offset + n] = (double)s / 32768.0; /* ‰¹ƒf[ƒ^‚ð-1ˆÈã1–¢–ž‚Ì”ÍˆÍ‚É³‹K‰»‚·‚é */
		}
	}

	fclose(fp);
}

void wave_write_IMA_ADPCM_mono(MONO_PCM *pcm, char *file_name) {
	FILE *fp;
	short block_size = 256;
	short samples_per_block = (block_size - 4) * 2 + 1;
	int number_of_block = (int)(pcm->length / samples_per_block);

	char riff_chunk_ID[4] = { 'R', 'I', 'F', 'F' };
	long riff_chunk_size = 52 + block_size * number_of_block;
	char file_format_type[4] = { 'W', 'A', 'V', 'E' };
	char fmt_chunk_ID[4] = { 'f', 'm', 't', ' ' };
	long fmt_chunk_size = 20;
	short wave_format_type = 17;
	short channel = 1;
	long samples_per_sec = pcm->fs; /* •W–{‰»Žü”g” */
	long bytes_per_sec = (long)(block_size * samples_per_sec / samples_per_block);
	short bits_per_sample = 4; /* —ÊŽq‰»¸“x */
	short extra_size = 2;

	char fact_chunk_ID[4] = { 'f', 'a', 'c', 't' };
	long fact_chunk_size = 4;
	long sample_length = samples_per_block * number_of_block + 1;
	char data_chunk_ID[4] = { 'd', 'a', 't', 'a' };
	long data_chunk_size = block_size * number_of_block;

	unsigned char c; /* 4bit‚Ìˆ³kƒf[ƒ^ */
	unsigned char header[4];
	unsigned char data;
	int sp, d, dp, block, index;

	static int index_table[16] =
	{
		-1, -1, -1, -1, 2, 4, 6, 8,
		-1, -1, -1, -1, 2, 4, 6, 8
	};

	static int step_size_table[89] =
	{
		7, 8, 9, 10, 11, 12, 13, 14,
		16, 17, 19, 21, 23, 25, 28, 31,
		34, 37, 41, 45, 50, 55, 60, 66,
		73, 80, 88, 97, 107, 118, 130, 143,
		157, 173, 190, 209, 230, 253, 279, 307,
		337, 371, 408, 449, 494, 544, 598, 658,
		724, 796, 876, 963, 1060, 1166, 1282, 1411,
		1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024,
		3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484,
		7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
		15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794,
		32767
	};

	fp = fopen(file_name, "wb");

	fwrite(riff_chunk_ID, 1, 4, fp);
	fwrite(&riff_chunk_size, 4, 1, fp);
	fwrite(file_format_type, 1, 4, fp);
	fwrite(fmt_chunk_ID, 1, 4, fp);
	fwrite(&fmt_chunk_size, 4, 1, fp);
	fwrite(&wave_format_type, 2, 1, fp);
	fwrite(&channel, 2, 1, fp);
	fwrite(&samples_per_sec, 4, 1, fp);
	fwrite(&bytes_per_sec, 4, 1, fp);
	fwrite(&block_size, 2, 1, fp);
	fwrite(&bits_per_sample, 2, 1, fp);
	fwrite(&extra_size, 2, 1, fp);
	fwrite(&samples_per_block, 2, 1, fp);
	fwrite(fact_chunk_ID, 1, 4, fp);
	fwrite(&fact_chunk_size, 4, 1, fp);
	fwrite(&sample_length, 4, 1, fp);
	fwrite(data_chunk_ID, 1, 4, fp);
	fwrite(&data_chunk_size, 4, 1, fp);

	for (block = 0; block < number_of_block; block++) {
		int offset = samples_per_block * block;

		for (int n = 0; n < samples_per_block; n++) {
			double x = (pcm->s[offset + n] + 1.0) / 2.0 * 65536.0;

			if (x > 65535.0) {
				x = 65535.0; /* クリッピング */
			}
			else if (x < 0.0) {
				x = 0.0; /* クリッピング */
			}

			short s = (short)((int)(x + 0.5) - 32768); /* ŽlŽÌŒÜ“ü‚ÆƒIƒtƒZƒbƒg‚Ì’²ß */

			if (block == 0 && n == 0) {
				index = 0; /* Å‰‚ÌƒuƒƒbƒN‚É‚¨‚¯‚éindex‚Ì‰Šú’l‚ð0‚É‚·‚é */
			}

			if (n == 0)	{
				header[0] = (unsigned char)(s & 0x00FF); /* s‚Ì‰ºˆÊƒoƒCƒg */
				header[1] = (unsigned char)((s >> 8) & 0x00FF); /* s‚ÌãˆÊƒoƒCƒg */
				header[2] = (unsigned char)index; /* ƒCƒ“ƒfƒbƒNƒX */
				header[3] = 0;

				fwrite(header, 1, 4, fp); /* ƒwƒbƒ_‚Ì‘‚«o‚µ */

				sp = s; /* sp‚Ì‰Šú’l‚ðs‚É‚·‚é */
			}
			else {
				d = s - sp;
				if (d < 0) {
					c = 0x8;
					d = -d;
				}
				else {
					c = 0x0;
				}

				int step_size = step_size_table[index];

				/* ˆ³k */
				if (d >= step_size) {
					c |= 0x4;
					d -= step_size;
				}
				if (d >= (step_size >> 1)) {
					c |= 0x2;
					d -= (step_size >> 1);
				}
				if (d >= (step_size >> 2)) {
					c |= 0x1;
				}

				/* L’£ */
				dp = step_size >> 3;
				if ((c & 0x1) == 0x1) {
					dp += (step_size >> 2);
				}
				if ((c & 0x2) == 0x2) {
					dp += (step_size >> 1);
				}
				if ((c & 0x4) == 0x4) {
					dp += step_size;
				}

				if ((c & 0x8) == 0x8) {
					sp -= dp;
				}
				else {
					sp += dp;
				}

				if (sp > 32767) {
					sp = 32767;
				}
				else if (sp < -32768) {
					sp = -32768;
				}

				index += index_table[c];
				if (index < 0) {
					index = 0;
				}
				else if (index > 88) {
					index = 88;
				}

				if ((n % 2) == 1) {
					data = c & 0xF; /* data‚Ì‰ºˆÊ4bit */
				}
				else {
					data |= (c & 0xF) << 4; /* data‚ÌãˆÊ4bit */

					fwrite(&data, 1, 1, fp); /* ˆ³kƒf[ƒ^‚Ì‘‚«o‚µ */
				}
			}
		}
	}

	fclose(fp);
}
