#include "pwm-audio-player.hpp"

int main() {
    stdio_init_all();

    // Create and initialize the audio player instance
    PWM_audio_player player(audio_data, AUDIO_PIN, SAMPLE_RATE);
    player.init();

    // Keep the core active while hardware interrupts process playback in the background
    while (true) {
        tight_loop_contents();
    }
}
