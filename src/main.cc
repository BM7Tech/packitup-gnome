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
#include "packitup_prefs.h"
#include <giomm.h>
#include <glib/gi18n.h>
#include <glibconfig.h>
#include <glibmm.h>
#include <glibmm/i18n.h>
#include <gtkmm.h>
#include <iostream>
#include <stdexcept>
extern "C"
{
#include <adwaita.h>
#include <gtk/gtk.h>
}
#include <gtkmm/init.h>

static void
on_action_preferences (GSimpleAction *action, GVariant *state,
                       gpointer user_data)
{
  try
    {
      GtkApplication *app = GTK_APPLICATION (user_data);
      GtkWindow *window_ref
          = gtk_application_get_active_window (GTK_APPLICATION (app));

      auto adw_window = ADW_APPLICATION_WINDOW (window_ref);
      auto prefs = PackitupPrefs::create (adw_window);
      prefs->present ();
    }
  catch (const Glib::Error &ex)
    {
      std::cerr << "Glib::Error: Packitup::on_action_preferences(): "
                << ex.what () << std::endl;
    }
  catch (const std::exception &ex)
    {
      std::cerr << "std::exception: Packitup::on_action_preferences(): "
                << ex.what () << std::endl;
    }
}

static void
on_action_about (GSimpleAction *action, GVariant *state, gpointer user_data)
{
  try
    {
      auto refBuilder = Gtk::Builder::create_from_resource (
          "/tech/bm7/packitup-gnome/src/about.ui");
      auto dialog = refBuilder->get_widget<Gtk::AboutDialog> ("about_window");

      GtkApplication *app = GTK_APPLICATION (user_data);
      GtkWindow *active_window
          = gtk_application_get_active_window (GTK_APPLICATION (app));
      auto cpp_wrap = Glib::wrap (active_window);

      dialog->set_transient_for (*cpp_wrap);
      dialog->set_program_name ("PackItUP!");
      dialog->set_copyright ("Copyright (C) 2025  edu-bm7 <edubm7@bm7.tech>");
      dialog->set_logo (Gdk::Texture::create_from_resource (
          "/tech/bm7/packitup-gnome/src/packitup.png"));
      dialog->set_license_type (Gtk::License::GPL_3_0);
      dialog->set_wrap_license (true);
      dialog->set_comments (_ ("Never run out of beer again."));
      dialog->set_website ("https://github.com/BM7Tech/packitup.git");
      dialog->set_website_label ("PackItUP! Website");
      std::vector<Glib::ustring> list_authors;
      list_authors.push_back ("edu-bm7 https://github.com/edu-bm7");
      list_authors.push_back ("BM7Tech https://github.com/BM7Tech");
      dialog->set_authors (list_authors);
      dialog->set_visible (true);
      dialog->present ();
      dialog->signal_hide ().connect ([dialog] () { delete dialog; });
    }
  catch (const Glib::Error &ex)
    {
      std::cerr << "Glib::Error: Packitup::on_action_about(): " << ex.what ()
                << std::endl;
    }
  catch (const std::exception &ex)
    {
      std::cerr << "std::exception: Packitup::on_action_about(): "
                << ex.what () << std::endl;
    }
}

static void
on_action_quit (GSimpleAction *action, GVariant *state, gpointer user_data)
{
  GtkApplication *app = GTK_APPLICATION (user_data);
  auto windows = gtk_application_get_windows (app);
  GList *window;
  window = windows;
  while (window)
    {
      gtk_widget_set_visible (GTK_WIDGET (window), false);
      window = window->next;
    }

  g_application_quit (G_APPLICATION (app));
}

static void
activate_callback (GApplication *gapp, gpointer user_data)
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

  g_autoptr (AdwApplication) app = NULL;
  static GActionEntry app_entries[] = {
    { "preferences", on_action_preferences, NULL, NULL, NULL },
    { "about", on_action_about, NULL, NULL, NULL },
    { "quit", on_action_quit, NULL, NULL, NULL },
  };
  const char *quit_accels[2] = { "<Ctrl>Q", NULL };

  gtk_init ();
  Gtk::init_gtkmm_internals ();
  app = adw_application_new ("tech.bm7.packitup-gnome",
                             G_APPLICATION_DEFAULT_FLAGS);

  auto packitup = Packitup::create (app);

  g_action_map_add_action_entries (G_ACTION_MAP (app), app_entries,
                                   G_N_ELEMENTS (app_entries), app);

  gtk_application_set_accels_for_action (GTK_APPLICATION (app), "app.quit",
                                         quit_accels);

  g_signal_connect (app, "activate", G_CALLBACK (activate_callback),
                    packitup.get ());

  return g_application_run (G_APPLICATION (app), argc, argv);
}
// vim: sts=2 sw=2 et
