#include <MagewellCapture.h>

#include <cassert>
#include <cstdlib>
#include <iostream>

using namespace magewell_capture;
using namespace std;

int test_open_channel(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  init_instance();
  refresh_device();

  for (auto &&channel : channel::iterator()) {
    cout << "Opening channel index: " << channel.index() << endl;
    cout << "  Device path: " << channel.get_device_path() << endl;

    // Open the channel.
    auto opened = channel.open();
    cout << "  Opened channel info:" << endl;

    auto info = opened.get_info();
    cout << "    Family Name: " << info.family_name << endl;
    cout << "    Product Name: " << info.product_name << endl;
    cout << "    Firmware Name: " << info.firmware_name << endl;
    cout << "    Board Serial No: " << info.board_serial_no << endl;
    cout << "    Family ID: " << info.family_id << endl;
    cout << "    Product ID: " << info.product_id << endl;
    cout << "    Hardware Version: " << static_cast<int>(info.hardware_version) << endl;
    cout << "    Firmware ID: " << static_cast<int>(info.firmware_id) << endl;
    cout << "    Firmware Version: " << hex << info.firmware_version << endl;
    cout << "    Driver Version: " << hex << info.driver_version << endl;

    // C++ automatically closes the channel when 'opened' channel goes out of scope.
  }

  return EXIT_SUCCESS;
}
