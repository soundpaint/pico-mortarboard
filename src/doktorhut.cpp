#include <cstdio>
#include <span>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/timer.h"

// Define our hardware pin and sample rate
inline constexpr uint AUDIO_PIN = 15;
inline constexpr uint32_t SAMPLE_RATE = 8000; // 8 kHz audio

// Dummy 8-bit audio data (Sine wave lookup table placeholder)
// 'const' + 'constexpr' ensures this is evaluated and stored in Flash memory
inline constexpr uint8_t audio_data[] = {
    128, 153, 177, 199, 219, 234, 246, 253, 255, 253, 246, 234, 219, 199, 177, 153,
    128, 102,  78,  56,  36,  21,   9,   2,   0,   2,   9,  21,  36,  56,  78, 102
};

class PWMAudioPlayer {
public:
    // Pass a view (span) of the flash memory buffer to the player
    explicit PWMAudioPlayer(std::span<const uint8_t> buffer, uint gpio_pin, uint32_t sample_rate)
        : audio_buffer_(buffer), pin_(gpio_pin), sample_rate_(sample_rate) {}

    void init() {
        // Setup GPIO pin for PWM
        gpio_set_function(pin_, GPIO_FUNC_PWM);
        slice_num_ = pwm_gpio_to_slice_num(pin_);
        channel_ = pwm_gpio_to_channel(pin_);

        // Wrap at 255 so 8-bit values map directly to 0-100% duty cycle
        pwm_set_wrap(slice_num_, 255);
        pwm_set_enabled(slice_num_, true);

        // Calculate exact delay in microseconds. 
        // A negative value forces the hardware timer to align exactly to grid intervals
        int32_t delay_us = -(1000000 / static_cast<int32_t>(sample_rate_));

        // Start the hardware repeating timer, passing 'this' as user data
        add_repeating_timer_us(delay_us, &PWMAudioPlayer::timer_callback_static, this, &timer_);
    }

private:
    // C-compatible wrapper function required by the SDK timer API
    static bool timer_callback_static(repeating_timer_t* t) {
        // Cast the generic user data pointer back to our C++ instance
        auto* instance = static_cast<PWMAudioPlayer*>(t->user_data);
        return instance->update_sample();
    }

    // Actual C++ instance method handling the logic
    bool update_sample() {
        if (audio_buffer_.empty()) return false;

        // Output sample
        pwm_set_chan_level(slice_num_, channel_, audio_buffer_[index_]);

        // Wrap around seamlessly
        index_ = (index_ + 1) % audio_buffer_.size();

        return true; // Return true to keep the timer active
    }

    std::span<const uint8_t> audio_buffer_;
    uint pin_;
    uint32_t sample_rate_;
    uint slice_num_{0};
    uint channel_{0};

    // volatile ensures the ISR always pulls fresh indexing data
    volatile size_t index_{0};
    repeating_timer_t timer_{};
};

int main() {
    stdio_init_all();

    // Create and initialize the audio player instance
    PWMAudioPlayer player(audio_data, AUDIO_PIN, SAMPLE_RATE);
    player.init();

    // Keep the core active while hardware interrupts process playback in the background
    while (true) {
        tight_loop_contents();
    }
}
