#include <MagewellCapture.h>

#include <cstdlib>

int test_refresh_device(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  // Call refresh_device to verify it works without error.
  // This works even without instance initialisation if the driver is not loaded.
  magewell_capture::init_instance();
  magewell_capture::refresh_device();
  magewell_capture::exit_instance();

  return EXIT_SUCCESS;
}
