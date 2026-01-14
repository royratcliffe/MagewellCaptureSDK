#include <MagewellCapture.h>

#include <cstdlib>

int test_exit_instance(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  // Call exit_instance multiple times to verify idempotency.
  //
  // In this case, set up a pair of atexit handlers to call
  // exit_instance at program termination, to verify that multiple calls
  // are safe. Set a breakpoint on the magewell_capture::exit_instance
  // function to verify. The following run in reverse order.
  atexit([]() { magewell_capture::exit_instance(); });
  atexit(magewell_capture::exit_instance);
  magewell_capture::exit_instance();

  return EXIT_SUCCESS;
}
