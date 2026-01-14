#include <MagewellCapture.h>

#include "MWCapture.h"
#include "MWEcoCapture.h"

#include <stdexcept>

using namespace magewell_capture;

uint32_t magewell_capture::get_version() {
  BYTE major = 0, minor = 0;
  WORD build = 0;
  // At version 3.3.1, the function MWGetVersion is a static inline
  // function defined in MWCapture.h; it always answers MW_SUCCEEDED.
  // Ignore the return value.
  (void)MWGetVersion(&major, &minor, &build);
  return (static_cast<uint32_t>(major) << 24) |
         (static_cast<uint32_t>(minor) << 16) | static_cast<uint32_t>(build);
}

void magewell_capture::init_instance() {
  if (MWCaptureInitInstance() == FALSE)
    throw std::runtime_error("MWCaptureInitInstance failed");
}
