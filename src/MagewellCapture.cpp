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
static void handle_result(MW_RESULT result, const char *what_function);

//! Copies fields of magewell_capture::channel::info from MWCAP_CHANNEL_INFO.
//! \param lhs The C++ info structure to fill in.
//! \param rhs The C structure to read from.
static void copy(magewell_capture::channel::info &lhs, const MWCAP_CHANNEL_INFO &rhs);

uint32_t magewell_capture::get_version() {
  BYTE major = 0xff, minor = 0xff;
  WORD build = 0xffff;
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

void magewell_capture::refresh_device() { handle_result(MWRefreshDevice(), "MWRefreshDevice"); }

int magewell_capture::channel::get_count() { return MWGetChannelCount(); }

magewell_capture::channel::info magewell_capture::channel::get_info() const {
  // Rely on RVO, return-value optimisation, to avoid copy.
  info info;
  MWCAP_CHANNEL_INFO info_;
  handle_result(MWGetChannelInfoByIndex(index_, &info_), "MWGetChannelInfoByIndex");
  copy(info, info_);
  return info;
}

std::string magewell_capture::channel::get_device_path() const {
  // Buffer size BUFSIZ should be more than enough. The examples in the
  // documentation use 128 bytes. Assume that Magewell correctly null terminates
  // the path.
  char device_path[BUFSIZ];
  handle_result(MWGetDevicePath(index_, device_path), "MWGetDevicePath");
  return std::string(device_path);
}

magewell_capture::channel::opened magewell_capture::channel::open() const {
  // Translates the channel index to a handle by getting the device path
  // first. Get the device path. Open the channel by path.
  return magewell_capture::open_channel(get_device_path());
}

magewell_capture::channel::opened magewell_capture::open_channel(const std::string &path) {
  // Open the channel by path.
  HCHANNEL handle = MWOpenChannelByPath(path.c_str());
  if (handle == NULL)
    throw std::runtime_error("MWOpenChannelByPath failed");
  return channel::opened(handle);
}

magewell_capture::channel::opened::~opened() {
  // Close the channel. The channel handle is stored as a void pointer. There is
  // no return value to check.
  MWCloseChannel(static_cast<HCHANNEL>(handle_));
}

magewell_capture::channel::info magewell_capture::channel::opened::get_info() const {
  // Get the channel information by handle.
  MWCAP_CHANNEL_INFO info_;
  handle_result(MWGetChannelInfo(static_cast<HCHANNEL>(handle_), &info_), "MWGetChannelInfo");
  info info;
  copy(info, info_);
  return info;
}

static void handle_result(MW_RESULT result, const char *what_function) {
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

static void copy(magewell_capture::channel::info &lhs, const MWCAP_CHANNEL_INFO &rhs) {
  // Fill in the fields of info from the C information structure.
  // Assign string fields directly; they are null-terminated.
  lhs.family_id = rhs.wFamilyID;
  lhs.product_id = rhs.wProductID;
  lhs.hardware_version = rhs.chHardwareVersion;
  lhs.firmware_id = rhs.byFirmwareID;
  lhs.firmware_version = rhs.dwFirmwareVersion;
  lhs.driver_version = rhs.dwDriverVersion;
  lhs.family_name = rhs.szFamilyName;
  lhs.product_name = rhs.szProductName;
  lhs.firmware_name = rhs.szFirmwareName;
  lhs.board_serial_no = rhs.szBoardSerialNo;
  lhs.board_index = rhs.byBoardIndex;
  lhs.channel_index = rhs.byChannelIndex;
}
