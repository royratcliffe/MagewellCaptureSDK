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
  }

  return EXIT_SUCCESS;
}
