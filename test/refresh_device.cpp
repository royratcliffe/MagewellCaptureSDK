#include <MagewellCapture.h>

#include <cstdlib>

int test_refresh_device(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  // Call refresh_device to verify it works without error.
  magewell_capture::refresh_device();

  return EXIT_SUCCESS;
}
