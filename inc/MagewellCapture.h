#ifndef MAGEWELL_CAPTURE_H
#define MAGEWELL_CAPTURE_H
#ifdef __cplusplus
extern "C" {
#endif

// for standard C integer types
#include <cstdint>

namespace magewell_capture {

//! Returns the version of the Magewell Capture SDK as a 32-bit integer.
//! The version number is encoded as follows:
//! - Bits 31-24: Major version
//! - Bits 23-16: Minor version
//! - Bits 15-0 : Build number
uint32_t get_version();

} // namespace magewell_capture

#ifdef __cplusplus
}
#endif
#endif // MAGEWELL_CAPTURE_H
