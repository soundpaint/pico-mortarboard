#include "audio.hpp"
#include <span>

class PWM_audio_player {
public:
    // pass a view (span) of the flash memory buffer to the player
    explicit PWM_audio_player(uint gpio_pin_audio, uint32_t sample_rate);

    void init();

private:
    // C-compatible wrapper function required by the SDK timer API
    static bool timer_callback_static(repeating_timer_t* t);

    // sample value generation
    uint8_t get_sine_wave_sample_value(const uint64_t index) const;
    uint8_t get_square_wave_sample_value(const uint64_t index) const;
    uint8_t get_wee_woo_wee_woo_sample_value(const uint64_t index) const;

    // actual C++ instance method handling the logic
    bool update_sample();

    std::span<const uint8_t> sine_wave_audio_buffer_;
    std::span<const uint8_t> square_wave_audio_buffer_;
    uint gpio_pin_audio_;
    uint32_t sample_rate_;
    uint slice_num_{0};
    uint channel_{0};

    // volatile ensures the ISR always pulls fresh indexing data
    volatile size_t index_{0};
    repeating_timer_t timer_{};
};
