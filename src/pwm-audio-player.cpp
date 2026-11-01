#include "pwm-audio-player.hpp"
#include <algorithm>
#include "hardware/pwm.h"

PWM_audio_player::PWM_audio_player() {}

void
PWM_audio_player::init_audio_out_pin() {
    // set drive strength to 12mA
    gpio_set_drive_strength(GPIO_PIN_AUDIO_OUT, GPIO_DRIVE_STRENGTH_12MA);

    // setup GPIO pin for PWM
    gpio_set_function(GPIO_PIN_AUDIO_OUT, GPIO_FUNC_PWM);
    slice_num_ = pwm_gpio_to_slice_num(GPIO_PIN_AUDIO_OUT);
    channel_ = pwm_gpio_to_channel(GPIO_PIN_AUDIO_OUT);
}

void
PWM_audio_player::init_wave_selection_pins() {
    // initialize GPIO wave select pins
    gpio_init(GPIO_PIN_WAVE_SELECT_1);
    gpio_init(GPIO_PIN_WAVE_SELECT_2);

    // set direction as input
    gpio_set_dir(GPIO_PIN_WAVE_SELECT_2, GPIO_IN);

    // enable internal pull-down resistor
    gpio_pull_down(GPIO_PIN_WAVE_SELECT_1);
    gpio_pull_down(GPIO_PIN_WAVE_SELECT_2);
}

void
PWM_audio_player::init() {
    sine_wave_audio_buffer_ = sine_wave_audio_data;
    square_wave_audio_buffer_ = square_wave_audio_data;

    // init GPIO pins
    init_audio_out_pin();
    init_wave_selection_pins();

    // wrap at 255 so 8-bit values map directly to 0-100% duty cycle
    pwm_set_wrap(slice_num_, 255);
    pwm_set_enabled(slice_num_, true);

    /*
     * Calculate exact delay in microseconds.  A negative value forces
     * the hardware timer to align exactly to grid intervals.
     */
    const int32_t delay_us = -(1000000 / static_cast<int32_t>(SAMPLE_RATE));

    // start the hardware repeating timer, passing 'this' as user data
    add_repeating_timer_us(delay_us, &PWM_audio_player::timer_callback_static,
                           this, &timer_);
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

uint8_t
PWM_audio_player::get_siren_sample_value(const uint64_t index) const {
    const uint64_t cyclic_index = 0x2000 - (index & 0x1fff);
    const uint64_t stretched_index = (cyclic_index * cyclic_index) >> 7;
    return (stretched_index & 0x1f) < 0x10 ? 0 : 255;
}

void
PWM_audio_player::check_wave_selections() {
    wave_select_1_ = gpio_get(GPIO_PIN_WAVE_SELECT_1);
    wave_select_2_ = gpio_get(GPIO_PIN_WAVE_SELECT_2);
}

// actual C++ instance method handling the logic
bool
PWM_audio_player::update_sample() {
    if ((index_ & 0xff) == 0) {
        check_wave_selections();
    }

    //const uint8_t sample_value = get_square_wave_sample_value(index_);
    const uint16_t sample_value =
        (wave_select_1_ ? get_wee_woo_wee_woo_sample_value(index_) : 0) +
        (wave_select_2_ ? get_siren_sample_value(index_) : 0);

    // output sample
    const uint8_t clamped_sample_value =
        std::clamp(sample_value, uint16_t(0), uint16_t(255));
    pwm_set_chan_level(slice_num_, channel_, clamped_sample_value);

    index_ = index_ + 1;

    return true; // return true to keep the timer active
}
