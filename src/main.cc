#include "packitup.h"

int
main (int argc, char *argv[])
{
  auto application = Packitup::create ();

  return application->run (argc, argv);
}
// vim: sts=2 sw=2 et
