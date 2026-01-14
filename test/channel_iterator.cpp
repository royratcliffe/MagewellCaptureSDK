#include <MagewellCapture.h>

#include <cassert>
#include <cstdlib>
#include <iostream>

int test_channel_iterator(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  magewell_capture::init_instance();
  magewell_capture::refresh_device();
  std::cout << "Number of channels: " << magewell_capture::channel::count()
            << std::endl;
  assert(magewell_capture::channel::count() >= 0);

  for (auto &&channel : magewell_capture::channel::iterator()) {
    std::cout << "Channel index: " << channel.index() << std::endl;
    assert(channel.index() >= 0);
    assert(channel.index() < magewell_capture::channel::count());

    // Get and print channel information.
    auto info = channel.get_info();
    std::cout << "  Family ID: " << info.family_id << std::endl;
    std::cout << "  Product ID: " << info.product_id << std::endl;
    std::cout << "  Hardware version: "
              << static_cast<int>(info.hardware_version) << std::endl;
    std::cout << "  Firmware ID: " << static_cast<int>(info.firmware_id)
              << std::endl;
    std::cout << "  Firmware version: " << info.firmware_version << std::endl;
    std::cout << "  Driver version: " << info.driver_version << std::endl;
    std::cout << "  Family name: " << info.family_name << std::endl;
    std::cout << "  Product name: " << info.product_name << std::endl;
    std::cout << "  Firmware name: " << info.firmware_name << std::endl;
    std::cout << "  Board serial no: " << info.board_serial_no << std::endl;
    std::cout << "  Board index: " << static_cast<int>(info.board_index)
              << std::endl;
    std::cout << "  Channel index: " << static_cast<int>(info.channel_index)
              << std::endl;
  }

  return EXIT_SUCCESS;
}
