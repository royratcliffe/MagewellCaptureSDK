#include <MagewellCapture.h>

#include <cstdlib>

int test_init_instance(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  magewell_capture::init_instance();
  return EXIT_SUCCESS;
}
