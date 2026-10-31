#include "pwm-audio-player.hpp"
#include "hardware/pwm.h"

// Pass a view (span) of the flash memory buffer to the player
PWM_audio_player::PWM_audio_player(uint gpio_pin_audio, uint32_t sample_rate)
    : gpio_pin_audio_(gpio_pin_audio), sample_rate_(sample_rate) {}

void
PWM_audio_player::init() {
    sine_wave_audio_buffer_ = sine_wave_audio_data;
    square_wave_audio_buffer_ = square_wave_audio_data;

    // Set drive strength to 12mA
    gpio_set_drive_strength(gpio_pin_audio_, GPIO_DRIVE_STRENGTH_12MA);

    // Setup GPIO pin for PWM
    gpio_set_function(gpio_pin_audio_, GPIO_FUNC_PWM);
    slice_num_ = pwm_gpio_to_slice_num(gpio_pin_audio_);
    channel_ = pwm_gpio_to_channel(gpio_pin_audio_);

    // Wrap at 255 so 8-bit values map directly to 0-100% duty cycle
    pwm_set_wrap(slice_num_, 255);
    pwm_set_enabled(slice_num_, true);

    /*
     * Calculate exact delay in microseconds.  A negative value forces
     * the hardware timer to align exactly to grid intervals.
     */
    const int32_t delay_us = -(1000000 / static_cast<int32_t>(sample_rate_));

    // start the hardware repeating timer, passing 'this' as user data
    add_repeating_timer_us(delay_us, &PWM_audio_player::timer_callback_static, this, &timer_);
}

// C-compatible wrapper function required by the SDK timer API
bool
PWM_audio_player::timer_callback_static(repeating_timer_t* t) {
    // cast the generic user data pointer back to our C++ instance
    auto* instance = static_cast<PWM_audio_player*>(t->user_data);
    return instance->update_sample();
}

uint8_t
PWM_audio_player::get_sine_wave_sample_value(const uint64_t index) const {
    return sine_wave_audio_buffer_[index & 0x1f];
}

uint8_t
PWM_audio_player::get_square_wave_sample_value(const uint64_t index) const {
    return square_wave_audio_buffer_[index & 0x1f];
}

uint8_t
PWM_audio_player::get_wee_woo_wee_woo_sample_value(const uint64_t index) const {
  const bool phase = index & 0x1000;
  if (phase) {
    return index & 0x08 ? 0 : 255;
  }
  const uint8_t rem = index % 66;
  if (rem < 11) return 0;
  if (rem < 22) return 255;
  if (rem < 33) return 0;
  if (rem < 44) return 255;
  if (rem < 55) return 0;
  return 255;
}

// actual C++ instance method handling the logic
bool
PWM_audio_player::update_sample() {
    //const uint8_t sample_value = get_square_wave_sample_value(index_);
    const uint8_t sample_value = get_wee_woo_wee_woo_sample_value(index_);

    // output sample
    pwm_set_chan_level(slice_num_, channel_, sample_value);

    index_ = index_ + 1;

    return true; // return true to keep the timer active
}
