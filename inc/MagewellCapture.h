//! \file MagewellCapture.h
//! \brief Declaration of Magewell Capture SDK C++ wrapper functions.

#ifndef MAGEWELL_CAPTURE_H
#define MAGEWELL_CAPTURE_H
#ifdef __cplusplus

// for standard C integer types
#include <cstdint>

#include <ranges>

namespace magewell_capture {

//! Returns the version of the Magewell Capture SDK as a 32-bit integer.
//! The version number is encoded as follows:
//! - Bits 31-24: Major version
//! - Bits 23-16: Minor version
//! - Bits 15-0 : Build number
uint32_t get_version();

//! Initialises a Magewell Capture instance.
//! Invoke this at the start of the program. It works idempotently; calling it
//! multiple times has no additional effect.
//! Throws std::runtime_error if initialisation fails.
void init_instance();

//! Cleans up the Magewell Capture instance.
//! Invoke this at the end of the program. Like init_instance, it works
//! idempotently; calling it multiple times has no additional effect.
void exit_instance();

//! Refreshes the device list.
//! Call this function when devices are plugged in or unplugged.
void refresh_device();

//! Opaque handle to a capture channel.
//! A simple integer identifies the channel.
class channel {
  const int index_;

public:
  explicit channel(int index) : index_(index) {}
  int index() const { return index_; }

  //! Returns the total number of channels.
  //! There are no channels if no devices are connected.
  static int count();

  //! Returns a channel by its index.
  //! The index must be in the range [0, count()).
  static channel at(int index) { return channel(index); }

  //! Returns a view of all channel indices.
  static auto range() { return std::views::iota(0, count()); };

  //! Returns a view of all channels.
  static auto iterator() {
    return range() | std::views::transform([](int i) { return channel(i); });
  }

  //! Information about a capture channel.
  //! Corresponds to MWCAP_CHANNEL_INFO.
  struct info {
    uint16_t family_id;
    uint16_t product_id;
    uint8_t hardware_version;
    uint8_t firmware_id;
    uint32_t firmware_version;
    uint32_t driver_version;
    std::string family_name;
    std::string product_name;
    std::string firmware_name;
    std::string board_serial_no;
    uint8_t board_index;
    uint8_t channel_index;
  };

  //! Gets information about the channel.
  //! Throws std::runtime_error on failure.
  info get_info() const;
};

} // namespace magewell_capture

#endif // __cplusplus
#endif // MAGEWELL_CAPTURE_H
