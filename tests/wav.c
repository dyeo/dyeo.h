#define WAV_IMPLEMENTATION
#include "../wav.h"
#include "../test.h"

#include <math.h>

int16_t gen_440hz_sine(uint32_t channels,
                       uint32_t sampleRate,
                       uint32_t sampleCount,
                       int32_t channelIndex,
                       int32_t sampleIndex)
{
  (void) channels;
  (void) sampleCount;
  (void) channelIndex;
  float t = (float) sampleIndex / (float) sampleRate;
  return wav_float32_to_pcm16(sinf(2.0f * 440.0f * 3.1415 * t) * 0.1f);
}

// -----------------------------------------------------------------------------
int main()
{
  test_group(wav, {
    wav_audio *sine = wav_gen_pcm16(2, wav_44100hz, 44100 * 5, gen_440hz_sine);
    test_true(sine != NULL);
    test_true(wav_play(sine));
    test_true(wav_dumpf("sine.wav", sine));
  });
}