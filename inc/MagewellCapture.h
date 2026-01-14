#ifndef MAGEWELL_CAPTURE_H
#define MAGEWELL_CAPTURE_H
#ifdef __cplusplus

// for standard C integer types
#include <cstdint>

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

} // namespace magewell_capture

#endif // __cplusplus
#endif // MAGEWELL_CAPTURE_H
