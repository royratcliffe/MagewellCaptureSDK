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
//! multiple times has no additional effect. Starts up a device-monitoring thread.
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
  //! Constructs a channel with the given index.
  explicit channel(int index) : index_(index) {}

  //! Returns the index of the channel.
  int index() const { return index_; }

  //! Returns the total number of channels.
  //! There are no channels if no devices are connected.
  static int get_count();

  //! Returns a channel by its index.
  //! The index must be in the range [0, count()).
  static channel at(int index) { return channel(index); }

  //! Returns a view of all channel indices.
  static auto range() { return std::views::iota(0, get_count()); };

  //! Returns a view of all channels.
  static auto iterator() {
    return range() | std::views::transform([](int i) { return channel(i); });
  }

  //! Information about a capture channel.
  //! Corresponds to MWCAP_CHANNEL_INFO.
  struct info {
    uint16_t family_id;          //!< Family identifier: MWCAP_FAMILY_ID.
    uint16_t product_id;         //!< Product identifier: MWCAP_PRODUCT_ID.
    uint8_t hardware_version;    //!< Hardware version.
    uint8_t firmware_id;         //!< Firmware identifier.
    uint32_t firmware_version;   //!< Firmware version.
    uint32_t driver_version;     //!< Driver version.
    std::string family_name;     //!< Family name.
    std::string product_name;    //!< Product name.
    std::string firmware_name;   //!< Firmware name.
    std::string board_serial_no; //!< Board serial number.
    uint8_t board_index;         //!< Board index.
    uint8_t channel_index;       //!< Channel index.
  };

  //! Gets information about the channel.
  //! Throws std::runtime_error on failure.
  info get_info() const;

  //! Gets the device instance path of the channel.
  //! Throws std::runtime_error on failure.
  std::string get_device_path() const;
};

} // namespace magewell_capture

#endif // __cplusplus
#endif // MAGEWELL_CAPTURE_H
