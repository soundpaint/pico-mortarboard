#include "pico/stdlib.h"

// define our hardware pin and sample rate
inline constexpr uint AUDIO_PIN = 1;
inline constexpr uint32_t SAMPLE_RATE = 8000; // 8 kHz audio

/*
 * Dummy 8-bit audio data (sine wave lookup table placeholder).
 * 'const' + 'constexpr' ensures this is evaluated and stored in flash
 * memory.
 */
inline constexpr uint8_t sine_wave_audio_data[] = {
    128, 153, 177, 199, 219, 234, 246, 253, 255, 253, 246, 234, 219, 199, 177, 153,
    128, 102,  78,  56,  36,  21,   9,   2,   0,   2,   9,  21,  36,  56,  78, 102
};

/*
 * Dummy 8-bit audio data (square wave lookup table placeholder).
 * 'const' + 'constexpr' ensures this is evaluated and stored in flash
 * memory.
 */
inline constexpr uint8_t square_wave_audio_data[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255
};
