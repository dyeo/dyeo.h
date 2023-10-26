/*
# wav.h

## Description

A single-header, easy-to-use .wav file encoder and decoder. Currently supports
16- and 32-bit PCM, and 32-bit float. The decoder utomatically de-interlaces the
audio data into separate channels for easy modification. Additionally, methods
for modifying and generating wav data are also available, and all formats are
inter-convertible.

## Usage

To use this library, simply include the provided header file in your project.
Depending on your platform (Windows or Linux), appropriate system headers and
definitions will be used.

```c
#include "wav.h"
```

In one C file in your project, you should define WAV_IMPLEMENTATION before
including the header, in order to actually include the implementation in your
compilation unit.

```c
#define WAV_IMPLEMENTATION
#include "wav.h"
```

## API Documentation

### Data Structures

1. `wav_t` - Represents a wav audio file, including its format, channels,
sample rate, sample count, and data.
2. `wavtype_t` - Enumerated type defining various wav formats.
3. `wavformat_t` - Enumerated type defining various wav audio formats.
4. `wavrate_t` - Enumerated type for supported sample rates.
5. `wavbits_t` - Enumerated type for supported bits per sample values.

### Conversion Functions

1. Conversion between PCM32, PCM16, float32, and ADPCM4.
    - `wav_pcm32_to_pcm16`
    - `wav_float32_to_pcm16`
    - `wav_pcm16_to_pcm32`
    - `wav_float32_to_pcm32`
    - `wav_pcm16_to_float32`
    - `wav_pcm32_to_float32`
    - `wav_adpcm4_to_pcm32`
    - `wav_pcm16_to_adpcm4`

### Encoding and Decoding

1. Conversion to specific wav formats:
    - `wav_to_pcm16`
    - `wav_to_pcm32`
    - `wav_to_float32`

2. Load and dump wav from and to files:
    - `wav_loadf`
    - `wav_dumpf`

3. Load and dump wav from and to byte arrays:
    - `wav_loadb`
    - `wav_dumpb`

4. Release memory allocated for a `wav_t` structure:
    - `wav_free`

### Playback

1. Play wav audio:
    - `wav_play`

2. Play wav audio asynchronously:
    - `wav_play_async`

### Generation

1. Generate wav audio from callbacks:
    - `wav_gen_pcm16`
    - `wav_gen_pcm32`
    - `wav_gen_float32`

The generation functions use callback mechanisms to fetch samples. Three
callback types are provided:
    - `wav_gen_pcm16_callback`
    - `wav_gen_pcm32_callback`
    - `wav_gen_float32_callback`

## Notes

1. Ensure you free any `wav_t` structures using the `wav_free` function to
avoid memory leaks.
2. When generating wav data using the provided generation functions, ensure your
callbacks provide valid sample data for the specified format.
*/
#ifndef _WAV_H
#define _WAV_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined(_WIN64)
#define WAV_WIN32 1
#ifndef WIN32_MEAN_AND_LEAN
#define WIN32_MEAN_AND_LEAN
#endif
#ifndef NOGDI
#define NOGDI
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <windows.h>
#else
#define WAV_LINUX 1
#include <alsa/asoundlib.h>
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -----------------------------------------------------------------------------

#define WAV_FORMAT_X_LIST                                                      \
  X(pcm16, int16_t, wav_pcm, false, int16_t)                                   \
  X(pcm32, int32_t, wav_pcm, false, int32_t)                                   \
  X(float32, float, wav_float, false, float)                                   \
  X(adpcm4, int16_t, wav_adpcm, true, uint8_t)

#define X(NAME, ...) wav_##NAME,
typedef enum wavtype_t
{
  WAV_FORMAT_X_LIST
} wavtype_t;
#undef X

typedef struct wav_t
{
  wavtype_t format;
  uint16_t channels;
  uint32_t sampleRate;
  uint32_t sampleCount;
  union
  {
#define X(NAME, TYPE, ...) TYPE **NAME;
    WAV_FORMAT_X_LIST
#undef X
  };
} *wav_t;

#define WAV_AUDIO_FOREACH(AUDIO, C, S)                                         \
  (uint32_t C = 0, S = 0; S < (AUDIO)->sampleCount;                            \
   S += C, C++, C %= (AUDIO)->channels)

// -----------------------------------------------------------------------------

#define wav_invalid 0

#define WAV_AUDIOFORMAT_X_LIST                                                 \
  X(wav_pcm, 0x0001)                                                           \
  X(wav_adpcm, 0x0002)                                                         \
  X(wav_float, 0x0003)                                                         \
  X(wav_extended, 0xFFFE)

typedef enum wavformat_t
{
#define X(A, B) A = B,
  WAV_AUDIOFORMAT_X_LIST
#undef X
} wavformat_t;

static const char *WAVFORMATTYPE_names[] = {
#define X(A, ...) #A,
  WAV_AUDIOFORMAT_X_LIST
#undef X
};

static wavformat_t WAVFORMAT_lookup[] = {
#define X(NAME, TYPE, FORMAT, ...) FORMAT,
  WAV_FORMAT_X_LIST
#undef X
};

typedef enum wavrate_t
{
  wav_8000hz   = 8000,
  wav_11025hz  = 11025,
  wav_16000hz  = 16000,
  wav_22050hz  = 22050,
  wav_32000hz  = 32000,
  wav_44100hz  = 44100,
  wav_48000hz  = 48000,
  wav_88200hz  = 88200,
  wav_96000hz  = 96000,
  wav_176400hz = 176400,
  wav_192000hz = 192000,
  wav_384000hz = 384000
} wavrate_t;

typedef enum wavbits_t
{
  wav_4bit   = 4,
  wav_8bit   = 8,
  wav_16bit  = 16,
  wav_24bit  = 24,
  wav_32bit  = 32,
  wav_48bit  = 48,
  wav_64bit  = 64,
  wav_128bit = 128,
} wavbits_t;

// -----------------------------------------------------------------------------

extern int16_t wav_pcm32_to_pcm16(int32_t sample);
extern int16_t wav_float32_to_pcm16(float sample);
extern int32_t wav_pcm16_to_pcm32(int16_t sample);
extern int32_t wav_float32_to_pcm32(float sample);
extern float wav_pcm16_to_float32(int16_t sample);
extern float wav_pcm32_to_float32(int32_t sample);
extern int16_t wav_adpcm4_to_pcm32(const uint8_t adpcm_sample,
                                   int32_t *predictor, int32_t *index);
extern uint8_t wav_pcm16_to_adpcm4(const int16_t pcm_sample, int32_t *predictor,
                                   int32_t *index);

// -----------------------------------------------------------------------------

extern void wav_to_pcm16(wav_t wav);
extern void wav_to_pcm16(wav_t wav);
extern void wav_to_pcm32(wav_t wav);
extern void wav_to_float32(wav_t wav);

// -----------------------------------------------------------------------------

extern wav_t wav_loadf(const char *filename);
extern bool wav_dumpf(const char *filename, const wav_t wav);

extern wav_t wav_loadb(size_t len, uint8_t *data);
extern uint8_t *wav_dumpb(const wav_t wav, size_t *len);

bool wav_play(const wav_t wav);
bool wav_play_async(const wav_t wav);

extern void wav_free(wav_t wav);

// -----------------------------------------------------------------------------

typedef int16_t (*wav_gen_pcm16_callback)(uint32_t channels,
                                          uint32_t sampleRate,
                                          uint32_t sampleCount,
                                          int32_t channelIndex,
                                          int32_t sampleIndex);

typedef int32_t (*wav_gen_pcm32_callback)(uint32_t channels,
                                          uint32_t sampleRate,
                                          uint32_t sampleCount,
                                          int32_t channelIndex,
                                          int32_t sampleIndex);
typedef float (*wav_gen_float32_callback)(uint32_t channels,
                                          uint32_t sampleRate,
                                          uint32_t sampleCount,
                                          int32_t channelIndex,
                                          int32_t sampleIndex);

wav_t wav_gen_pcm16(uint32_t channels, uint32_t sampleRate,
                    uint32_t sampleCount, wav_gen_pcm16_callback callback);

wav_t wav_gen_pcm32(uint32_t channels, uint32_t sampleRate,
                    uint32_t sampleCount, wav_gen_pcm32_callback callback);

wav_t wav_gen_float32(uint32_t channels, uint32_t sampleRate,
                      uint32_t sampleCount, wav_gen_float32_callback callback);

// -----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif

#ifdef WAV_IMPLEMENTATION
#ifndef _WAV_C
#define _WAV_C

// -----------------------------------------------------------------------------

#define WAV_INTERLEAVE(I_FRAMES, I_TYPE, O_FRAMES, O_TYPE, CHANNELS,           \
                       FRAMECOUNT)                                             \
  do                                                                           \
  {                                                                            \
    O_FRAMES = (O_TYPE *) malloc(sizeof(O_TYPE) * CHANNELS * FRAMECOUNT);      \
    if (O_FRAMES == NULL)                                                      \
      break;                                                                   \
    for (size_t i = 0; i < FRAMECOUNT; ++i)                                    \
    {                                                                          \
      for (size_t ch = 0; ch < CHANNELS; ++ch)                                 \
      {                                                                        \
        (O_FRAMES)[(i * CHANNELS) + ch] = (I_FRAMES)[ch][i];                   \
      }                                                                        \
    }                                                                          \
  } while (0)

#define WAV_DEINTERLEAVE(I_FRAMES, I_TYPE, O_FRAMES, O_TYPE, CHANNELS,         \
                         FRAMECOUNT)                                           \
  do                                                                           \
  {                                                                            \
    O_FRAMES = (O_TYPE **) malloc(sizeof(O_TYPE *) * CHANNELS);                \
    if (O_FRAMES == NULL)                                                      \
      break;                                                                   \
    for (size_t ch = 0; ch < CHANNELS; ++ch)                                   \
    {                                                                          \
      (O_FRAMES)[ch] = (O_TYPE *) malloc(sizeof(O_TYPE) * FRAMECOUNT);         \
      if ((O_FRAMES)[ch] == NULL)                                              \
      {                                                                        \
        for (size_t j = 0; j < ch; ++j)                                        \
        {                                                                      \
          free((O_FRAMES)[j]);                                                 \
        }                                                                      \
        free(O_FRAMES);                                                        \
        O_FRAMES = NULL;                                                       \
        break;                                                                 \
      }                                                                        \
      for (size_t i = 0; i < FRAMECOUNT; ++i)                                  \
      {                                                                        \
        (O_FRAMES)[ch][i] = (I_FRAMES)[(i * CHANNELS) + ch];                   \
      }                                                                        \
    }                                                                          \
  } while (0)

#define WAV_POPVAL(VALUE)                                                      \
  do                                                                           \
  {                                                                            \
    memcpy(&(VALUE), &(data)[i], sizeof(VALUE));                               \
    i += sizeof(VALUE);                                                        \
  } while (0)

#define WAV_PUSHVAL(VALUE)                                                     \
  do                                                                           \
  {                                                                            \
    memcpy(&(data)[i], &(VALUE), sizeof(VALUE));                               \
    i += sizeof(VALUE);                                                        \
  } while (0)

#define WAV_POPBYTES(BYTES, LEN)                                               \
  do                                                                           \
  {                                                                            \
    BYTES = malloc(LEN);                                                       \
    memcpy((BYTES), &(data)[i], (LEN));                                        \
    i += (LEN);                                                                \
  } while (0)

#define WAV_PUSHBYTES(BYTES, LEN)                                              \
  do                                                                           \
  {                                                                            \
    memcpy(&(data)[i], (BYTES), (LEN));                                        \
    i += (LEN);                                                                \
  } while (0)

#define WAV_TESTBYTES(BYTES, LEN, BLOCK)                                       \
  do                                                                           \
  {                                                                            \
    if (memcmp(&(data)[i], BYTES, LEN))                                        \
    {                                                                          \
      BLOCK;                                                                   \
    }                                                                          \
    i += LEN;                                                                  \
  } while (0)

// -----------------------------------------------------------------------------

int16_t wav_pcm32_to_pcm16(int32_t sample)
{
  return (int16_t) (sample >> 16);
}

int16_t wav_float32_to_pcm16(float sample)
{
  return (int16_t) (sample * ((float) 0x7FFF));
}

#define _wav_decode_pcm16(...) NULL
#define _wav_encode_pcm16(...) NULL

int32_t wav_pcm16_to_pcm32(int16_t sample)
{
  return ((int32_t) sample) << 16;
}

int32_t wav_float32_to_pcm32(float sample)
{
  return (int32_t) (sample * ((float) 0x7FFFFFFF));
}

#define _wav_decode_pcm32(...) NULL
#define _wav_encode_pcm32(...) NULL

float wav_pcm16_to_float32(int16_t sample)
{
  return sample / (float) 0x7FFF;
}

float wav_pcm32_to_float32(int32_t sample)
{
  return sample / (float) 0x7FFFFFFF;
}

#define _wav_decode_float32(...) NULL
#define _wav_encode_float32(...) NULL

// -----------------------------------------------------------------------------

const int32_t _wav_adpcm4_step_table[89] = {
  7,     8,     9,     10,    11,    12,    13,    14,    16,    17,
  19,    21,    23,    25,    28,    31,    34,    37,    41,    45,
  50,    55,    60,    66,    73,    80,    88,    97,    107,   118,
  130,   143,   157,   173,   190,   209,   230,   253,   279,   307,
  337,   371,   408,   449,   494,   544,   598,   658,   724,   796,
  876,   963,   1060,  1166,  1282,  1411,  1552,  1707,  1878,  2066,
  2272,  2499,  2749,  3024,  3327,  3660,  4026,  4428,  4871,  5358,
  5894,  6484,  7132,  7845,  8630,  9493,  10442, 11487, 12635, 13899,
  15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767};

const int32_t _wav_adpcm4_index_adjust[8] = {-1, -1, -1, -1, 2, 4, 6, 8};

int16_t wav_adpcm4_to_pcm32(const uint8_t adpcm_sample, int32_t *predictor,
                            int32_t *index)
{
  int32_t step = _wav_adpcm4_step_table[*index];
  int32_t diff = step >> 3;
  if (adpcm_sample & 1)
    diff += step >> 2;
  if (adpcm_sample & 2)
    diff += step >> 1;
  if (adpcm_sample & 4)
    diff += step;
  if (adpcm_sample & 8)
  {
    *predictor -= diff;
    if (*predictor < -32768)
      *predictor = -32768;
  }
  else
  {
    *predictor += diff;
    if (*predictor > 32767)
      *predictor = 32767;
  }
  *index += _wav_adpcm4_index_adjust[adpcm_sample & 7];
  if (*index < 0)
    *index = 0;
  if (*index > 88)
    *index = 88;
  return (int16_t) *predictor;
}

uint8_t wav_pcm16_to_adpcm4(const int16_t pcm_sample, int32_t *predictor,
                            int32_t *index)
{
  int32_t step         = _wav_adpcm4_step_table[*index];
  int32_t diff         = pcm_sample - *predictor;
  uint8_t adpcm_sample = 0;
  if (diff < 0)
  {
    adpcm_sample |= 0x08;
    diff = -diff;
  }
  int32_t mask = 4;
  for (int32_t i = 3; i > 0; i--)
  {
    if (diff >= step)
    {
      adpcm_sample |= mask;
      diff -= step;
    }
    step >>= 1;
    mask >>= 1;
  }
  wav_adpcm4_to_pcm32(adpcm_sample, predictor, index);
  return adpcm_sample;
}

int16_t *_wav_decode_adpcm4(const uint8_t *adpcm, size_t len, size_t *outlength)
{
  *outlength          = len * 2;
  int16_t *pcm_buffer = (int16_t *) malloc(*outlength * sizeof(int16_t));
  if (!pcm_buffer)
  {
    printf("Memory allocation failed!\n");
    return NULL;
  }
  int32_t predictor = 0;
  int32_t index     = 0;
  for (size_t i = 0; i < len; i++)
  {
    pcm_buffer[2 * i] =
      wav_adpcm4_to_pcm32((adpcm[i] >> 4) & 0x0F, &predictor, &index);
    pcm_buffer[2 * i + 1] =
      wav_adpcm4_to_pcm32(adpcm[i] & 0x0F, &predictor, &index);
  }
  return pcm_buffer;
}

uint8_t *_wav_encode_adpcm4(const int16_t *pcm, size_t length,
                            size_t *outlength)
{
  *outlength            = length / 2;
  uint8_t *adpcm_buffer = (uint8_t *) malloc(*outlength);
  if (!adpcm_buffer)
  {
    printf("Memory allocation failed!\n");
    return NULL;
  }
  int32_t predictor = 0;
  int32_t index     = 0;
  for (size_t i = 0; i < *outlength; i++)
  {
    uint8_t high_nibble = wav_pcm16_to_adpcm4(pcm[2 * i], &predictor, &index);
    uint8_t low_nibble =
      wav_pcm16_to_adpcm4(pcm[2 * i + 1], &predictor, &index);
    adpcm_buffer[i] = (high_nibble << 4) | low_nibble;
  }
  return adpcm_buffer;
}

// -----------------------------------------------------------------------------

wav_t wav_gen_pcm16(uint32_t channels, uint32_t sampleRate,
                    uint32_t sampleCount, wav_gen_pcm16_callback callback)
{
  if (channels == 0 || sampleRate == 0 || sampleCount == 0)
  {
    return NULL;
  }
  wav_t wav        = malloc(sizeof(struct wav_t));
  wav->format      = wav_pcm16;
  wav->channels    = channels;
  wav->sampleRate  = sampleRate;
  wav->sampleCount = sampleCount;
  wav->pcm16       = malloc(sizeof(int16_t *));
  for (uint32_t c = 0; c < channels; ++c)
  {
    wav->pcm32[c] = malloc(sampleCount * sizeof(int16_t));
    for (uint32_t i = 0; i < sampleCount; ++i)
    {
      if (callback)
      {
        wav->pcm16[c][i] = callback(channels, sampleRate, sampleCount, c, i);
      }
      else
      {
        wav->pcm16[c][i] = wav_float32_to_pcm16(0.0f);
      }
    }
  }
  return wav;
}

wav_t wav_gen_pcm32(uint32_t channels, uint32_t sampleRate,
                    uint32_t sampleCount, wav_gen_pcm32_callback callback)
{
  if (channels == 0 || sampleRate == 0 || sampleCount == 0)
  {
    return NULL;
  }
  wav_t wav        = malloc(sizeof(struct wav_t));
  wav->format      = wav_pcm32;
  wav->channels    = channels;
  wav->sampleRate  = sampleRate;
  wav->sampleCount = sampleCount;
  wav->pcm32       = malloc(sizeof(int32_t *));
  for (uint32_t c = 0; c < channels; ++c)
  {
    wav->pcm32[c] = malloc(sampleCount * sizeof(int32_t));
    for (uint32_t i = 0; i < sampleCount; ++i)
    {
      if (callback)
      {
        wav->pcm32[c][i] = callback(channels, sampleRate, sampleCount, c, i);
      }
      else
      {
        wav->pcm32[c][i] = wav_float32_to_pcm32(0.0f);
      }
    }
  }
  return wav;
}

wav_t wav_gen_float32(uint32_t channels, uint32_t sampleRate,
                      uint32_t sampleCount, wav_gen_float32_callback callback)
{
  if (channels == 0 || sampleRate == 0 || sampleCount == 0)
  {
    return NULL;
  }
  wav_t wav        = malloc(sizeof(struct wav_t));
  wav->format      = wav_float32;
  wav->channels    = channels;
  wav->sampleRate  = sampleRate;
  wav->sampleCount = sampleCount;
  wav->float32     = malloc(sizeof(float *));
  for (uint32_t c = 0; c < channels; ++c)
  {
    wav->pcm32[c] = malloc(sampleCount * sizeof(int32_t));
    for (uint32_t i = 0; i < sampleCount; ++i)
    {
      if (callback)
      {
        wav->float32[c][i] = callback(channels, sampleRate, sampleCount, c, i);
      }
      else
      {
        wav->float32[c][i] = 0.0f;
      }
    }
  }
  return wav;
}

// -----------------------------------------------------------------------------

void wav_to_pcm16(wav_t wav)
{
  if (!wav)
    return;
  int16_t **new_data = malloc(wav->channels * sizeof(int16_t *));
  if (new_data == NULL)
  {
    fprintf(stderr, "ERROR: Memory allocation error\n");
    free(new_data);
    return;
  }
  for (uint16_t c = 0; c < wav->channels; c++)
  {
    new_data[c] = malloc(wav->sampleCount * sizeof(int16_t));
    for (uint32_t i = 0; i < wav->sampleCount; i++)
    {
      switch (wav->format)
      {
        case wav_pcm32:
          new_data[c][i] = wav_pcm32_to_pcm16(wav->pcm32[c][i]);
          break;
        case wav_float32:
          new_data[c][i] = wav_float32_to_pcm16(wav->float32[c][i]);
          break;
        default:
          return;
      }
    }
  }
  for (uint16_t ch = 0; ch < wav->channels; ch++)
  {
    free(wav->pcm16[ch]);
  }
  free(wav->pcm16);
  wav->pcm16  = new_data;
  wav->format = wav_pcm16;
}

void wav_to_pcm32(wav_t wav)
{
  if (!wav || wav->format == wav_pcm32)
    return;
  int32_t **new_data = malloc(wav->channels * sizeof(int32_t *));
  if (new_data == NULL)
  {
    fprintf(stderr, "ERROR: Memory allocation error\n");
    free(new_data);
    return;
  }
  switch (wav->format)
  {
    default:
      return;
    case wav_pcm16:
    {
    for
      WAV_AUDIO_FOREACH(wav, c, i)
      {
        new_data[c][i] = wav_pcm16_to_pcm32(wav->pcm16[c][i]);
      }
    }
    break;
    case wav_pcm32:
    {
    for
      WAV_AUDIO_FOREACH(wav, c, i)
      {
        new_data[c][i] = wav_float32_to_pcm32(wav->pcm32[c][i]);
      }
    }
    break;
  }
  for (uint16_t ch = 0; ch < wav->channels; ch++)
  {
    free(wav->pcm32[ch]);
  }
  free(wav->pcm32);
  wav->pcm32  = new_data;
  wav->format = wav_pcm32;
}

void wav_to_float32(wav_t wav)
{
  if (!wav || wav->format == wav_float32)
    return;
  float **new_data = malloc(wav->channels * sizeof(float *));
  if (new_data == NULL)
  {
    fprintf(stderr, "ERROR: Memory allocation error\n");
    free(new_data);
    return;
  }
  for (uint16_t c = 0; c < wav->channels; c++)
  {
    new_data[c] = malloc(wav->sampleCount * sizeof(float));
    for (uint32_t i = 0; i < wav->sampleCount; i++)
    {
    switch (wav->format)
    {
      case wav_pcm16:
        new_data[c][i] = wav_pcm16_to_float32(wav->pcm16[c][i]);
        break;
      case wav_pcm32:
        new_data[c][i] = wav_pcm32_to_float32(wav->pcm32[c][i]);
        break;
      default:
        return;
    }
    }
  }
  for (uint16_t ch = 0; ch < wav->channels; ch++)
  {
    free(wav->float32[ch]);
  }
  free(wav->float32);
  wav->float32 = new_data;
  wav->format  = wav_float32;
}

// -----------------------------------------------------------------------------

wav_t wav_loadf(const char *filename)
{
  FILE *file = fopen(filename, "rb");
  if (file == NULL)
  {
    fprintf(stderr, "ERROR: Could not read file '%s'\n", filename);
    return NULL;
  }
  fseek(file, 0, SEEK_END);
  size_t len = ftell(file);
  fseek(file, 0, SEEK_SET);
  uint8_t *data = (uint8_t *) malloc(len);
  if (data == NULL)
  {
    fclose(file);
    return NULL;
  }
  size_t bytesRead = fread(data, 1, len, file);
  if (bytesRead != len)
  {
    fprintf(stderr, "ERROR: Could not read entire file '%s'\n", filename);
    free(data);
    fclose(file);
    return NULL;
  }
  wav_t wav = wav_loadb(len, data);
  fclose(file);
  return wav;
}

bool wav_dumpf(const char *filename, const wav_t wav)
{
  FILE *file = fopen(filename, "wb");
  if (file == NULL)
  {
    fprintf(stderr, "ERROR: Could not write to file '%s'\n", filename);
    return false;
  }
  size_t len;
  uint8_t *data = wav_dumpb(wav, &len);
  fwrite(data, sizeof(uint8_t), len, file);
  fclose(file);
  return true;
}

// -----------------------------------------------------------------------------

wav_t wav_loadb(size_t len, uint8_t *data)
{

  if (len < 44)
  {
    fprintf(stderr, "ERROR: wav_t header too small\n");
    return NULL;
  }
  wav_t wav = malloc(sizeof(struct wav_t));

  size_t i = 0;
  WAV_TESTBYTES("RIFF", 4, {
    fprintf(stderr, "ERROR: Invalid RIFF header '%.4s'\n", &data[i]);
    goto error;
  });
  uint32_t fileSizeMinusEight;
  WAV_POPVAL(fileSizeMinusEight);
  if (fileSizeMinusEight + 8 != len)
  {
    fprintf(stderr, "ERROR: Invalid chunk size '%u' (%lld)\n",
            fileSizeMinusEight,
            (ptrdiff_t) len - (ptrdiff_t) fileSizeMinusEight);
    goto error;
  }
  WAV_TESTBYTES("WAVE", 4, {
    fprintf(stderr, "ERROR: Invalid WAVE header '%.4s'\n", &data[i]);
    goto error;
  });
  WAV_TESTBYTES("fmt ", 4, {
    fprintf(stderr, "ERROR: Invalid format header '%.4s'\n", &data[i]);
    goto error;
  });

  uint32_t formatChunkSize;
  WAV_POPVAL(formatChunkSize);
  uint16_t audioFormat;
  WAV_POPVAL(audioFormat);
  if (audioFormat == wav_adpcm4 && formatChunkSize != 20)
  {
    fprintf(stderr, "ERROR: Invalid format chunk size '%u'\n", formatChunkSize);
    goto error;
  }

  uint16_t numChannels;
  WAV_POPVAL(numChannels);
  uint32_t sampleRate;
  WAV_POPVAL(sampleRate);
  uint32_t byteRate;
  WAV_POPVAL(byteRate);
  uint16_t blockAlign;
  WAV_POPVAL(blockAlign);
  uint16_t bitsPerSample;
  WAV_POPVAL(bitsPerSample);

  if (audioFormat == wav_adpcm)
  {
    uint16_t extraParamSize = 0;
    WAV_POPVAL(extraParamSize);
    if (extraParamSize != sizeof **(wav->adpcm4) * 16)
    {
    fprintf(stderr, "ERROR: Invalid ADPCM extra param size '%u'\n",
            extraParamSize);
    goto error;
    }
    uint16_t adpcmSamplesPerBlock = 0;
    WAV_POPVAL(adpcmSamplesPerBlock);
    WAV_TESTBYTES("fact", 4, {
      fprintf(stderr, "ERROR: Invalid fact header '%.4s'\n", &data[i]);
      goto error;
    });
    uint32_t adpcmBlockSize = 0;
    WAV_POPVAL(adpcmBlockSize);
    if (adpcmBlockSize != 4)
    {
    fprintf(stderr, "ERROR: Invalid ADPCM block size '%u'\n", adpcmBlockSize);
    goto error;
    }
    uint32_t adpcmActualSampleCount = 0;
    WAV_POPVAL(adpcmActualSampleCount);
    wav->sampleCount = adpcmActualSampleCount;
  }

  WAV_TESTBYTES("data", 4, {
    fprintf(stderr, "ERROR: Invalid data header '%.4s'\n", &data[i]);
    goto error;
  });

  uint32_t dataLength;
  WAV_POPVAL(dataLength);

#define X(NAME, TYPE, FORMAT, ...)                                             \
  if (audioFormat == FORMAT && bitsPerSample == (sizeof(TYPE) * 8))            \
  {                                                                            \
    wav->format = wav_##NAME;                                                  \
  }                                                                            \
  else
  WAV_FORMAT_X_LIST
#undef X
  {
    fprintf(stderr, "ERROR: wav_t format '%s%u' suspected but not supported\n",
            &WAVFORMATTYPE_names[audioFormat][4], bitsPerSample);
    goto error;
  }

  wav->channels    = numChannels;
  wav->sampleRate  = sampleRate;
  wav->sampleCount = dataLength / ((bitsPerSample / 8) * numChannels);

  switch (wav->format)
  {
    default:
    fprintf(stderr, "ERROR: Invalid data format\n");
    goto error;
#define X(NAME, TYPE, FORMAT, ENCODED, OUTTYPE)                                \
  case wav_##NAME:                                                             \
  {                                                                            \
    wav->format = wav_##NAME;                                                  \
    size_t blen = wav->channels * wav->sampleCount * sizeof(TYPE);             \
    TYPE *raw   = malloc(blen);                                                \
    WAV_POPBYTES(raw, blen);                                                   \
    if (ENCODED)                                                               \
    {                                                                          \
    size_t dlen;                                                               \
    TYPE *decoded    = _wav_decode_##NAME((OUTTYPE *) raw, blen, &dlen);       \
    raw              = decoded;                                                \
    wav->sampleCount = dlen;                                                   \
    }                                                                          \
    WAV_DEINTERLEAVE(raw, OUTTYPE, wav->NAME, TYPE, wav->channels,             \
                     wav->sampleCount);                                        \
    free(raw);                                                                 \
    break;                                                                     \
  }
    WAV_FORMAT_X_LIST
#undef X
  }

  return wav;

error:
  free(wav);
  return NULL;
}

uint8_t *wav_dumpb(const wav_t wav, size_t *len)
{
  uint32_t dataLength = 0;
  uint16_t audioFormat;
  uint16_t bitsPerSample   = 0;
  uint32_t formatChunkSize = 16;

  switch (wav->format)
  {
    default:
    fprintf(stderr, "ERROR: Invalid wav_t format\n");
    goto error;
#define X(NAME, TYPE, FORMAT, ENCODED, OUTTYPE)                                \
  case wav_##NAME:                                                             \
  {                                                                            \
    dataLength    = wav->channels * wav->sampleCount * sizeof(TYPE);           \
    audioFormat   = FORMAT;                                                    \
    bitsPerSample = sizeof(TYPE) * 8;                                          \
    break;                                                                     \
  }
    WAV_FORMAT_X_LIST
#undef X
  }

  if (wav->format == wav_adpcm4)
  {
    bitsPerSample   = 4;
    formatChunkSize = 20;
    dataLength += 6;
  }

  uint8_t *data = (uint8_t *) malloc(sizeof(uint8_t) * (44 + dataLength));
  if (!data)
  {
    fprintf(stderr, "ERROR: Memory allocation failure\n");
    *len = 0;
    return NULL;
  }

  size_t i = 0;
  WAV_PUSHBYTES("RIFF", 4);
  size_t block1SizeI          = i;
  uint32_t fileSizeMinusEight = 36 + dataLength;
  WAV_PUSHVAL(fileSizeMinusEight);
  WAV_PUSHBYTES("WAVE", 4);
  WAV_PUSHBYTES("fmt ", 4);
  WAV_PUSHVAL(formatChunkSize);
  WAV_PUSHVAL(audioFormat);
  uint16_t numChannels = wav->channels;
  WAV_PUSHVAL(numChannels);
  uint32_t sampleRate = wav->sampleRate;
  WAV_PUSHVAL(sampleRate);
  uint32_t byteRate = sampleRate * numChannels * (bitsPerSample / 8);
  WAV_PUSHVAL(byteRate);
  uint16_t blockAlign = numChannels * (bitsPerSample / 8);
  WAV_PUSHVAL(blockAlign);
  WAV_PUSHVAL(bitsPerSample);

  if (wav->format == wav_adpcm4)
  {
    uint16_t extraParamSize         = 2;
    uint16_t samplesPerBlock        = 2;
    uint32_t adpcmBlockSize         = 4;
    uint32_t adpcmActualSampleCount = wav->sampleCount;
    WAV_PUSHVAL(extraParamSize);
    WAV_PUSHVAL(samplesPerBlock);
    WAV_PUSHBYTES("fact", 4);
    WAV_PUSHVAL(adpcmBlockSize);
    WAV_PUSHVAL(adpcmActualSampleCount);
    dataLength -= 6;
  }

  WAV_PUSHBYTES("data", 4);

  switch (wav->format)
  {
    default:
    fprintf(stderr, "ERROR: Invalid export format\n");
    return NULL;
#define X(NAME, TYPE, FORMAT, ENCODED, OUTTYPE)                                \
  case wav_##NAME:                                                             \
  {                                                                            \
    OUTTYPE *raw = NULL;                                                       \
    WAV_INTERLEAVE(wav->NAME, TYPE, raw, OUTTYPE, wav->channels,               \
                   wav->sampleCount);                                          \
    if (ENCODED)                                                               \
    {                                                                          \
    size_t dlen      = 0;                                                      \
    OUTTYPE *encoded = _wav_encode_##NAME((TYPE *) raw, dataLength, &dlen);    \
    raw              = encoded;                                                \
    dataLength       = dlen;                                                   \
    }                                                                          \
    WAV_PUSHVAL(dataLength);                                                   \
    WAV_PUSHBYTES(raw, dataLength);                                            \
    free(raw);                                                                 \
    break;                                                                     \
  }
    WAV_FORMAT_X_LIST
#undef X
  }

  fileSizeMinusEight =
    36 + dataLength; // ensure filesize is correct if we change it
  memcpy(&data[block1SizeI], &fileSizeMinusEight, sizeof(fileSizeMinusEight));

  if (len)
  {
    *len = i;
  }

  return data;

error:
  if (len)
  {
    *len = 1;
  }
  return NULL;
}

// -----------------------------------------------------------------------------

#ifdef WAV_WIN32

static HGLOBAL _wav_win32_h_header;
static WAVEHDR _wav_win32_header;
static HWAVEOUT _wav_win32_h_wav_out;

void CALLBACK _pcm_out_win32(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance,
                             DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
  (void) hwo;
  (void) dwInstance;
  (void) dwParam1;
  (void) dwParam2;
  if (uMsg == WOM_DONE)
  {
    WAVEHDR *waveHeader = (WAVEHDR *) dwParam1;
    waveOutUnprepareHeader(_wav_win32_h_wav_out, &_wav_win32_header,
                           sizeof(WAVEHDR));
    free(waveHeader->lpData); // This frees the playData
    GlobalFree(_wav_win32_h_header);
    waveOutClose(_wav_win32_h_wav_out);
  }
}

char *_wav_deinterleave(char **channels, uint32_t channelCount,
                        uint32_t sampleCount, uint16_t bitDepth)
{
  int bytesPerSample  = bitDepth / 8;
  uint64_t outputSize = sampleCount * channelCount * bytesPerSample;
  char *output        = (char *) malloc(outputSize);
  if (!output)
  {
    return NULL;
  }
  for (uint32_t sample = 0; sample < sampleCount; ++sample)
  {
    for (uint32_t channel = 0; channel < channelCount; ++channel)
    {
    char *currentChannel = channels[channel];
    for (int byte = 0; byte < bytesPerSample; ++byte)
    {
      *output++ = currentChannel[sample * bytesPerSample + byte];
    }
    }
  }
  return output - outputSize;
}

void _wav_play_pcm(uint16_t format, uint16_t channels, uint32_t sampleRate,
                   uint32_t sampleCount, uint16_t bitDepth, char **data)
{
  WAVEFORMATEX wfx;
  uint64_t bufferLength = sampleCount * channels * (bitDepth / 8);
  wfx.wFormatTag        = format;
  wfx.nChannels         = channels;
  wfx.nSamplesPerSec    = sampleRate;
  wfx.wBitsPerSample    = bitDepth;
  wfx.nBlockAlign       = (channels * bitDepth) / 8;
  wfx.nAvgBytesPerSec   = wfx.nSamplesPerSec * wfx.nBlockAlign;
  wfx.cbSize            = 0;
  if (waveOutOpen(&_wav_win32_h_wav_out, WAVE_MAPPER, &wfx,
                  (DWORD_PTR) _pcm_out_win32, 0,
                  CALLBACK_FUNCTION) != MMSYSERR_NOERROR)
  {
    printf("ERROR: Failed to open wave out device\n");
    return;
  }
  _wav_win32_h_header =
    GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, sizeof(WAVEHDR));
  char *playData = _wav_deinterleave(data, channels, sampleCount, bitDepth);
  _wav_win32_header.lpData = playData;
  _wav_win32_header.dwBufferLength = bufferLength;
  waveOutPrepareHeader(_wav_win32_h_wav_out, &_wav_win32_header,
                       sizeof(WAVEHDR));
  waveOutWrite(_wav_win32_h_wav_out, &_wav_win32_header, sizeof(WAVEHDR));
}

#else

static snd_pcm_t *_wav_linux_pcm_handle;
static snd_pcm_uframes_t _wav_linux_frames;
static char *_wav_linux_playbuf;

void _wav_play_pcm(uint16_t format, uint16_t channels, uint32_t sampleRate,
                   uint32_t sampleCount, uint16_t bitDepth, char *data)
{
  int err;
  snd_pcm_hw_params_t *params;
  if ((err = snd_pcm_open(&_wav_linux_pcm_handle, "default",
                          SND_PCM_STREAM_PLAYBACK, 0)) < 0)
  {
    fprintf(stderr, "ERROR: Cannot open PCM device: %s\n", snd_strerror(err));
    return;
  }
  snd_pcm_hw_params_alloca(&params);
  snd_pcm_hw_params_any(_wav_linux_pcm_handle, params);
  if (format == wav_float)
  {
    snd_pcm_hw_params_set_format(_wav_linux_pcm_handle, params,
                                 SND_PCM_FORMAT_FLOAT_LE);
  }
  else
  {
    if (bitDepth == 16)
    {
    snd_pcm_hw_params_set_format(_wav_linux_pcm_handle, params,
                                 SND_PCM_FORMAT_S16_LE);
    }
    else if (bitDepth == 32)
    {
    snd_pcm_hw_params_set_format(_wav_linux_pcm_handle, params,
                                 SND_PCM_FORMAT_S32_LE);
    }
  }
  snd_pcm_hw_params_set_channels(_wav_linux_pcm_handle, params, channels);
  unsigned int sampleRate = sampleRate;
  snd_pcm_hw_params_set_rate_near(_wav_linux_pcm_handle, params, &sampleRate,
                                  NULL);
  snd_pcm_hw_params(_wav_linux_pcm_handle, params);
  snd_pcm_hw_params_get_period_size(params, &_wav_linux_frames, NULL);
  _wav_linux_playbuf = (char *) malloc(_wav_linux_frames * 2);
  while (sampleCount > 0)
  {
    int framesToWrite = sampleCount / (bitDepth / 8);
    if (framesToWrite > _wav_linux_frames)
    {
    framesToWrite = _wav_linux_frames;
    }
    memcpy(_wav_linux_playbuf, data, framesToWrite * (bitDepth / 8));
    data += framesToWrite * (bitDepth / 8);
    sampleCount -= framesToWrite * (bitDepth / 8);
    err =
      snd_pcm_writei(_wav_linux_pcm_handle, _wav_linux_playbuf, framesToWrite);
    if (err == -EPIPE)
    {
    snd_pcm_prepare(_wav_linux_pcm_handle);
    }
  }
  snd_pcm_drain(_wav_linux_pcm_handle);
  snd_pcm_close(_wav_linux_pcm_handle);
  free(_wav_linux_playbuf);
}

#endif

// -----------------------------------------------------------------------------

bool wav_play_async(const wav_t wav)
{
  if (wav == NULL)
  {
    return false;
  }
  switch (wav->format)
  {
    default:
    fprintf(stderr, "ERROR: Invalid data format\n");
    return false;
#define X(NAME, TYPE, FORMAT, ENCODED, OUTTYPE)                                \
  case wav_##NAME:                                                             \
  {                                                                            \
    _wav_play_pcm(WAVFORMAT_lookup[wav_##NAME], wav->channels,                 \
                  wav->sampleRate, wav->sampleCount, sizeof **(wav->NAME) * 8, \
                  (char **) wav->NAME);                                        \
    break;                                                                     \
  }
    WAV_FORMAT_X_LIST
#undef X
  }
  return true;
}

bool wav_play(const wav_t wav)
{
  bool playing = wav_play_async(wav);
  if (playing)
  {
    Sleep(((float) wav->sampleCount / (float) wav->sampleRate) * 1000);
  }
  return playing;
}

// -----------------------------------------------------------------------------

void wav_free(wav_t wav)
{
  (void) wav;
  switch (wav->format)
  {
    default:
    fprintf(stderr, "ERROR: Unknown wav_t format\n");
    return;
#define X(NAME, TYPE, FORMAT, ENCODED, OUTTYPE)                                \
  case wav_##NAME:                                                             \
  {                                                                            \
    for (uint16_t c = 0; c < wav->channels; ++c)                               \
    {                                                                          \
    free(wav->NAME[c]);                                                        \
    wav->NAME[c] = NULL;                                                       \
    }                                                                          \
    free(wav->NAME);                                                           \
    wav->NAME = NULL;                                                          \
    break;                                                                     \
  }
    WAV_FORMAT_X_LIST
#undef X
  }
  wav->format      = wav_invalid;
  wav->channels    = 0;
  wav->sampleRate  = wav_invalid;
  wav->sampleCount = 0;
  free(wav);
}

#undef WAV_INTERLEAVE
#undef WAV_DEINTERLEAVE
#undef WAV_POPBYTES
#undef WAV_POPVAL
#undef WAV_PUSHBYTES
#undef WAV_PUSHVAL

// -----------------------------------------------------------------------------

#endif
#endif
/*
This software is served under two licenses - pick which you prefer.
--------------------------------------------------------------------------------
MIT License
Copyright 2023 Dani Yeomans

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
--------------------------------------------------------------------------------
Public Domain
Dedicated 2023 Dani Yeomans

The author of this software and associated documentation files (the "Software")
dedicates any and all copyright interest in the Software to the public domain.
The author makes this dedication for the benefit of the public at large and to
the detriment of the author's heirs and successors. The author intends this
dedication to be an overt act of relinquishment in perpetuity of all present and
future rights to the Software under copyright law.

Any person obtaining a copy of the Software and associated documentation files
is free to to deal in the Software without restriction, including without
limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom
the Software is furnished to do so, subject to no conditions.
--------------------------------------------------------------------------------
*/
