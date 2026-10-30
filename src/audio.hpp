#include "pico/stdlib.h"

// Define our hardware pin and sample rate
inline constexpr uint AUDIO_PIN = 15;
inline constexpr uint32_t SAMPLE_RATE = 8000; // 8 kHz audio

// Dummy 8-bit audio data (Sine wave lookup table placeholder)
// 'const' + 'constexpr' ensures this is evaluated and stored in Flash memory
inline constexpr uint8_t audio_data[] = {
    128, 153, 177, 199, 219, 234, 246, 253, 255, 253, 246, 234, 219, 199, 177, 153,
    128, 102,  78,  56,  36,  21,   9,   2,   0,   2,   9,  21,  36,  56,  78, 102
};
