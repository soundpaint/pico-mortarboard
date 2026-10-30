#include "pwm-audio-player.hpp"
#include "hardware/pwm.h"

// Pass a view (span) of the flash memory buffer to the player
PWM_audio_player::PWM_audio_player(std::span<const uint8_t> buffer,
                                   uint gpio_pin, uint32_t sample_rate)
    : audio_buffer_(buffer), pin_(gpio_pin), sample_rate_(sample_rate) {}

void
PWM_audio_player::init() {
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
    add_repeating_timer_us(delay_us, &PWM_audio_player::timer_callback_static, this, &timer_);
}

// C-compatible wrapper function required by the SDK timer API
bool
PWM_audio_player::timer_callback_static(repeating_timer_t* t) {
    // Cast the generic user data pointer back to our C++ instance
    auto* instance = static_cast<PWM_audio_player*>(t->user_data);
    return instance->update_sample();
}

// Actual C++ instance method handling the logic
bool
PWM_audio_player::update_sample() {
    if (audio_buffer_.empty()) return false;

    // Output sample
    pwm_set_chan_level(slice_num_, channel_, audio_buffer_[index_]);

    // Wrap around seamlessly
    index_ = (index_ + 1) % audio_buffer_.size();

    return true; // Return true to keep the timer active
}
