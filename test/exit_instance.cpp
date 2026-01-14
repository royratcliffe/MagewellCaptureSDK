#include <MagewellCapture.h>

#include <cstdlib>

int test_exit_instance(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  // Call exit_instance multiple times to verify idempotency.
  atexit([]() { magewell_capture::exit_instance(); });
  atexit(magewell_capture::exit_instance);
  magewell_capture::exit_instance();

  return EXIT_SUCCESS;
}
