#include "audio.hpp"
#include <span>

class PWM_audio_player {
public:
  explicit PWM_audio_player();
  void init();

private:
  // C-compatible wrapper function required by the SDK timer API
  static bool timer_callback_static(repeating_timer_t* t);

  // GPIO initialization
  void init_audio_out_pin();
  void init_wave_selection_pins();
  void init_wave_led_pins();

  // sample value generation
  uint8_t update_sine_wave_sample_value(const uint64_t index);
  uint8_t update_square_wave_sample_value(const uint64_t index);
  uint8_t update_wee_woo_wee_woo_sample_value(const uint64_t index);
  uint8_t update_siren_sample_value(const uint64_t index);

  // actual C++ instance method handling the logic
  void check_wave_selections();
  bool update_sample();

  bool wave_select_1_ = 0;
  bool wave_select_2_ = 0;

  std::span<const uint8_t> sine_wave_audio_buffer_;
  std::span<const uint8_t> square_wave_audio_buffer_;
  uint slice_num_{0};
  uint channel_{0};

  // volatile ensures the ISR always pulls fresh indexing data
  volatile size_t index_{0};
  repeating_timer_t timer_{};
};
