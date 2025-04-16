#include "packitup.h"
#include <glibmm/i18n.h>

int
main (int argc, char *argv[])
{
  // Localized text
  setlocale (LC_ALL, "");
  bindtextdomain (GETTEXT_PACKAGE, LOCALE_DIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);

  auto application = Packitup::create ();

  return application->run (argc, argv);
}
// vim: sts=2 sw=2 et
