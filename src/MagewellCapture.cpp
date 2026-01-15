//! \file MagewellCapture.cpp
//! \brief Implementation of Magewell Capture SDK C++ wrapper functions.

#include <MagewellCapture.h>

#include "MWCapture.h"
#include "MWEcoCapture.h"

#include <stdexcept>

using namespace magewell_capture;

//! Throws std::runtime_error if result is not MW_SUCCEEDED.
//! The error message includes the function name passed as what_function.
//! \param result The MW_RESULT value to check.
//! \param what_function The name of the function that produced the result.
static void throw_if_not_succeeded(MW_RESULT result, const char *what_function);

uint32_t magewell_capture::get_version() {
  BYTE major = 0, minor = 0;
  WORD build = 0;
  // At version 3.3.1, the function MWGetVersion is a static inline
  // function defined in MWCapture.h; it always answers MW_SUCCEEDED.
  // Ignore the return value.
  (void)MWGetVersion(&major, &minor, &build);
  return (static_cast<uint32_t>(major) << 24) | (static_cast<uint32_t>(minor) << 16) | static_cast<uint32_t>(build);
}

void magewell_capture::init_instance() {
  // Call MWCaptureInitInstance; throw if it fails. It returns FALSE on failure;
  // does not use a result code.
  if (MWCaptureInitInstance() == FALSE)
    throw std::runtime_error("MWCaptureInitInstance failed");
}

void magewell_capture::exit_instance() { MWCaptureExitInstance(); }

void magewell_capture::refresh_device() { throw_if_not_succeeded(MWRefreshDevice(), "MWRefreshDevice"); }

int magewell_capture::channel::get_count() { return MWGetChannelCount(); }

magewell_capture::channel::info magewell_capture::channel::get_info() const {
  // Rely on RVO, return-value optimisation, to avoid copy.
  info info;
  MWCAP_CHANNEL_INFO info_;
  throw_if_not_succeeded(MWGetChannelInfoByIndex(index_, &info_), "MWGetChannelInfoByIndex");
  // Fill in the fields of info from c_info.
  // Assign string fields directly; they are null-terminated.
  info.family_id = info_.wFamilyID;
  info.product_id = info_.wProductID;
  info.hardware_version = info_.chHardwareVersion;
  info.firmware_id = info_.byFirmwareID;
  info.firmware_version = info_.dwFirmwareVersion;
  info.driver_version = info_.dwDriverVersion;
  info.family_name = info_.szFamilyName;
  info.product_name = info_.szProductName;
  info.firmware_name = info_.szFirmwareName;
  info.board_serial_no = info_.szBoardSerialNo;
  info.board_index = info_.byBoardIndex;
  info.channel_index = info_.byChannelIndex;
  return info;
}

static void throw_if_not_succeeded(MW_RESULT result, const char *what_function) {
  if (result != MW_SUCCEEDED)
    switch (result) {
    case MW_FAILED:
      throw std::runtime_error(std::string(what_function) + " failed");
    case MW_ENODATA:
      throw std::runtime_error(std::string(what_function) + " no data");
    case MW_INVALID_PARAMS:
      throw std::runtime_error(std::string(what_function) + " invalid");
    default:
      // Throw unknown errors for other codes though logically there
      // should be no more error types since the result enumeration only
      // defines the preceding ones. But be defensive.
      throw std::runtime_error(std::string(what_function) + " unknown error");
    }
}
