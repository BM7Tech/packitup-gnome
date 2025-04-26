/*
 * PackItUP! Never run out of beer again.
 * Copyright (C) 2025  edu-bm7 <edubm7@bm7.tech>
 *
 * This file is part of PackItUP!.
 *
 * PackItUP! is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PackItUP! is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PackItUP!. If not, see <https://www.gnu.org/licenses/>.
 * */

#include "packitup.h"
#include <giomm.h>
#include <glibconfig.h>
#include <glibmm.h>
#include <glibmm/i18n.h>
#include <gtkmm.h>
#include <stdexcept>
extern "C"
{
#include <adwaita.h>
#include <gtk/gtk.h>
}
#include <gtkmm/init.h>

void
startup_callback (GApplication * /*app*/, gpointer user_data)
{
  static_cast<Packitup *> (user_data)->on_startup ();
}

void
activate_callback (GApplication * /*app*/, gpointer user_data)
{
  static_cast<Packitup *> (user_data)->on_activate ();
}

int
main (int argc, char *argv[])
{
  // Localized text
  setlocale (LC_ALL, "");
  bindtextdomain (GETTEXT_PACKAGE, LOCALE_DIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);

  adw_init ();
  Gtk::init_gtkmm_internals ();
  g_autoptr (AdwApplication) app = NULL;

  app = adw_application_new ("tech.bm7.packitup-gnome",
                             G_APPLICATION_DEFAULT_FLAGS);

  auto packitup = Packitup::create (app);

  g_signal_connect (app, "startup", G_CALLBACK (startup_callback),
                    packitup.get ());
  g_signal_connect (app, "activate", G_CALLBACK (activate_callback),
                    packitup.get ());

  return g_application_run (G_APPLICATION (app), argc, argv);
}
// vim: sts=2 sw=2 et
