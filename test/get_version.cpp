#include <MagewellCapture.h>

#include <cassert>
#include <iostream>

int test_get_version(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  const auto version = magewell_capture::get_version();
  const auto major = (version >> 24) & 0xff;
  const auto minor = (version >> 16) & 0xff;
  const auto build = version & 0xffff;

  // Cast to int to avoid printing as char.
  std::cout << "Magewell Capture SDK Version: " << static_cast<int>(major)
            << "." << static_cast<int>(minor) << "." << build << std::endl;

  // Note that the build number 0x05eb is 1515 in decimal.
  assert(version == 0x030305ebU);
  return EXIT_SUCCESS;
}
