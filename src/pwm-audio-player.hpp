#include "audio.hpp"
#include <span>

class PWM_audio_player {
public:
    // Pass a view (span) of the flash memory buffer to the player
    explicit PWM_audio_player(std::span<const uint8_t> buffer,
                              uint gpio_pin, uint32_t sample_rate);

    void init();

private:
    // C-compatible wrapper function required by the SDK timer API
    static bool timer_callback_static(repeating_timer_t* t);

    // Actual C++ instance method handling the logic
    bool update_sample();

    std::span<const uint8_t> audio_buffer_;
    uint pin_;
    uint32_t sample_rate_;
    uint slice_num_{0};
    uint channel_{0};

    // volatile ensures the ISR always pulls fresh indexing data
    volatile size_t index_{0};
    repeating_timer_t timer_{};
};
