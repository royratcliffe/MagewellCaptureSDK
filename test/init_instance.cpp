#include <MagewellCapture.h>

#include <cstdlib>

int test_init_instance(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  // Call init_instance multiple times to verify idempotency.
  magewell_capture::init_instance();
  magewell_capture::init_instance();
  magewell_capture::init_instance();
  return EXIT_SUCCESS;
}
